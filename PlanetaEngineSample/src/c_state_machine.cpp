#include <cassert>

#include "boost/algorithm/string.hpp"

#include "c_state_machine.hpp"

using namespace plnt;

namespace {
	template <typename T>
	void separate_state_path_to_state_id_hierarchy(T &dst, const std::string &state_path) {
		boost::split(dst, state_path, boost::is_any_of("."));
	}
}

//////////////////////////////////////////////////////////////////////////
//CStateMachine
//////////////////////////////////////////////////////////////////////////

PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_state_machine) {
	registerer
		//.ShallowCopyTarget(&CStateMachine::state_data_map_)
		.shallow_copy_target(&c_state_machine::state_data_list_);
}

c_state_machine::c_state_machine(): state_data_list_(1) { state_data_list_[0].state_hierarchy_depth = 0; }

bool c_state_machine::parent_base_transition(const state_data &parent_state_data,
                                           std::list<std::string> &ids) {
	const auto target_state_depth = parent_state_data.state_hierarchy_depth + 1;
	assert(target_state_depth > 0);
	//auto& current_state_data = state_data_list_[target_state_depth - 1];
	auto nxt_state_id = ids.front();
	const auto it = parent_state_data.child_state_data_idxes.find(nxt_state_id);
	if (it == parent_state_data.child_state_data_idxes.end()) {
		PE_LOG_ERROR("ステート\"", parent_state_data.state_id, "\"の子ステートとしてステート\"", nxt_state_id, "\"は存在しません。");
		return false;
	}
	auto nxt_state_data_idx = it->second;
	const auto &nxt_state_data = state_data_list_[nxt_state_data_idx];
	ids.pop_front();
	//深度が深いステートから順に終了処理を実行していく
	for (auto i = static_cast<signed>(active_state_list_.size()) - 1; i >= static_cast<signed>(target_state_depth) - 1;
	     --i) { active_state_list_[i].second->exit_proc_base(); }
	//目的の深度までに実行ステートリストをリサイズ
	active_state_list_.resize(target_state_depth);
	//新しいステートを作成
	auto new_state = nxt_state_data.creator();
	active_state_list_[target_state_depth - 1] = std::make_pair(nxt_state_data_idx, new_state);

	if (ids.empty()) {
		if (auto trans_id = new_state->
			enter_proc_base()) { return me_base_transition(nxt_state_data, *trans_id); } else { return true; }
	}
	//子ステートIDの指定がある場合は状態開始処理の戻り値は無視する
	new_state->enter_proc_base();
	//子ステートの遷移
	return parent_base_transition(nxt_state_data, ids);
}

bool c_state_machine::me_base_transition(const state_data &state_data, const std::string &relative_state_path) {
	assert(state_data.state_hierarchy_depth > 0);
	if (relative_state_path.empty()) { return false; }
	std::string modified_rel_s_path = relative_state_path;
	if (relative_state_path[0] == '.') { modified_rel_s_path = state_data.state_id + relative_state_path; }
	std::list<std::string> ids;
	separate_state_path_to_state_id_hierarchy(ids, modified_rel_s_path);
	return parent_base_transition(state_data_list_[state_data.parent_state_data_idx], ids);
}

bool c_state_machine::register_state(const std::function<std::shared_ptr<state_base>()> &creator,
                                   const std::string &state_path) {
	std::vector<std::string> id_hierarchy;
	separate_state_path_to_state_id_hierarchy(id_hierarchy, state_path);
	if (id_hierarchy.empty()) {
		PE_LOG_ERROR("ステートパス\"", state_path, "\"へのステート登録に失敗しました。パスが空の可能性があります。");
		return false;
	}
	//親の探索
	size_t parent_state_data_idx = 0;
	for (int i = 0; i < static_cast<signed>(id_hierarchy.size()) - 1; ++i) {
		auto &cld_dat_idxes = state_data_list_[parent_state_data_idx].child_state_data_idxes;
		if (auto cld_dat_idx_map_it = cld_dat_idxes.find(id_hierarchy[i]); cld_dat_idx_map_it == cld_dat_idxes.end()) {
			std::string parent_state_path;
			for (int j = 0; j < i - 1; ++j) { parent_state_path += id_hierarchy[j] + "."; }
			parent_state_path += id_hierarchy[i - 1];
			PE_LOG_ERROR("ステートパス\"", state_path, "\"へのステート登録に失敗しました。ステートパス\"", parent_state_path, "\"の子にIDが\"",
			             id_hierarchy[i], "\"であるステートは存在しません。");
			return false;
		} else { parent_state_data_idx = cld_dat_idx_map_it->second; }
	}
	//親を指定して登録
	return register_state_with_parent_state_data_idx(creator, *(--id_hierarchy.end()), parent_state_data_idx);
}

bool c_state_machine::register_state_with_parent_state_data_idx(
	const std::function<std::shared_ptr<state_base>()> &creator,
	const std::string &state_id, size_t parent_state_data_idx) {
	state_data sd;
	sd.state_id = state_id;
	sd.creator = creator;
	sd.state_hierarchy_depth = state_data_list_[parent_state_data_idx].state_hierarchy_depth + 1;
	//親ステートのインデックスを設定
	sd.parent_state_data_idx = parent_state_data_idx;
	//ステートマップでの存在確認
	/*if (state_data_map_.find(state_id) != state_data_map_.end()) {
		PE_LOG_WARNING("すでに存在するステート\"", state_id, "\"が指定されたため追加しませんでした。");
		return true;
	}*/
	//ステートデータリストに登録してインデックスを取得
	state_data_list_.push_back(sd);
	//インデックスを親ステートデータに登録
	if (auto idx = state_data_list_.size() - 1; !std::get<bool>(
		state_data_list_[parent_state_data_idx].child_state_data_idxes.emplace(state_id, idx))) {
		PE_LOG_WARNING("すでに存在するステート\"", state_id, "\"が指定されたため上書きされました。");
	}
	//インデックスをステートデータマップに登録
	//state_data_map_.emplace(state_id, idx);
	return true;
}

bool c_state_machine::start_state_machine(const std::string &id) { return transition(id); }

void c_state_machine::stop_state_machine() {
	for (auto it = active_state_list_.rbegin(); it != active_state_list_.rend(); ++it) { it->second->exit_proc_base(); }
	active_state_list_.clear();
}

bool c_state_machine::transition(const std::string &state_id) { return transition_impl(state_id); }

bool c_state_machine::transition_impl(const std::string &state_id) {
	std::list<std::string> ids;
	separate_state_path_to_state_id_hierarchy(ids, state_id);
	return parent_base_transition(state_data_list_[0], ids);
}

bool c_state_machine::send_message(const std::string &message) {
	if (active_state_list_.empty()) {
		PE_LOG_ERROR("ステートマシンが開始されていません。");
		return false;
	}
	int d = 0;
	for (auto &&stt : active_state_list_) {
		if (auto trans_id = stt.second->message_handler_base(message)) {
			return me_base_transition(state_data_list_[stt.first], *trans_id);
		}
		++d;
	}
	return true;
}

void c_state_machine::on_initialized() {
	super::on_initialized();
	const auto task = game_object().create_and_attach_task<t_instant>(task_slot::pre_collision_early_phase);
	task->set_execute_function([this] { update(); });
}

void c_state_machine::update() {
	int d = 0;
	for (auto &&stt : active_state_list_) {
		if (auto ts = stt.second->update_proc_base()) {
			me_base_transition(state_data_list_[stt.first], *ts);
			break;
		}
		++d;
	}
}
