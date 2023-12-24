//MSVC�ɂ�����boost.split��c4996���
#if defined(_MSC_VER) && _MSC_VER >= 1400 
#pragma warning(push) 
#pragma warning(disable:4996) 
#endif 

#include <cassert>

#include "boost/algorithm/string.hpp"

#include "CStateMachine.hpp"

using namespace plnt;

namespace {
	template<typename Cntnr>
	void SeparateStatePathToStateIdHierarchy(Cntnr& dst, const std::string& state_path) {
		boost::split(dst, state_path, boost::is_any_of("."));
	}
}

//////////////////////////////////////////////////////////////////////////
//CStateMachine
//////////////////////////////////////////////////////////////////////////

PE_REFLECTION_DATA_REGISTERER_DEFINITION(CStateMachine) {
	registerer
		//.ShallowCopyTarget(&CStateMachine::state_data_map_)
		.ShallowCopyTarget(&CStateMachine::state_data_list_);
}

CStateMachine::CStateMachine():state_data_list_(1) {
	state_data_list_[0].state_hierarchy_depth = 0;
}

bool CStateMachine::ParentBaseTransition_(const CStateMachine::StateData& parent_state_data, std::list<std::string>& ids) {
	auto target_state_depth = parent_state_data.state_hierarchy_depth + 1;
	assert(target_state_depth > 0);
	//auto& current_state_data = state_data_list_[target_state_depth - 1];
	auto nxt_state_id = ids.front();
	auto it = parent_state_data.child_state_data_idxes.find(nxt_state_id);
	if (it == parent_state_data.child_state_data_idxes.end()) {
		PE_LOG_ERROR("�X�e�[�g\"", parent_state_data.state_id, "\"�̎q�X�e�[�g�Ƃ��ăX�e�[�g\"", nxt_state_id, "\"�͑��݂��܂���B");
		return false;
	}
	auto nxt_state_data_idx = it->second;
	auto& nxt_state_data = state_data_list_[nxt_state_data_idx];
	ids.pop_front();
	//�[�x���[���X�e�[�g���珇�ɏI�����������s���Ă���
	for (auto i = (signed)active_state_list_.size() - 1; i >= (signed)target_state_depth - 1; --i) {
		active_state_list_[i].second->ExitProcBase();
	}
	//�ړI�̐[�x�܂łɎ��s�X�e�[�g���X�g�����T�C�Y
	active_state_list_.resize(target_state_depth);
	//�V�����X�e�[�g���쐬
	auto new_state = nxt_state_data.creator();
	active_state_list_[target_state_depth - 1] = std::make_pair(nxt_state_data_idx, new_state);
	
	if (ids.empty()) {
		auto trans_id = new_state->EnterProcBase();
		if(trans_id) {
			return MeBaseTransition_(nxt_state_data, *trans_id);
		} else {
			return true;
		}
	} else {
		//�q�X�e�[�gID�̎w�肪����ꍇ�͏�ԊJ�n�����̖߂�l�͖�������
		new_state->EnterProcBase();
		//�q�X�e�[�g�̑J��
		return ParentBaseTransition_(nxt_state_data, ids);
	}
}

bool CStateMachine::MeBaseTransition_(const StateData& state_data, const std::string& relative_state_path) {
	assert(state_data.state_hierarchy_depth > 0);
	if (relative_state_path.empty()) {
		return false;
	}
	std::string modified_r_spathl = relative_state_path;
	if (relative_state_path[0] == '.') {
		modified_r_spathl = state_data.state_id + relative_state_path;
	}
	std::list<std::string> ids;
	SeparateStatePathToStateIdHierarchy(ids, modified_r_spathl);
	return ParentBaseTransition_(state_data_list_[state_data.parent_state_data_idx], ids);
}

bool CStateMachine::RegisterState_(const std::function<std::shared_ptr<StateBase>()>& creator, const std::string& state_path) {
	std::vector<std::string> id_hierarchy;
	SeparateStatePathToStateIdHierarchy(id_hierarchy, state_path);
	if (id_hierarchy.empty()) {
		PE_LOG_ERROR("�X�e�[�g�p�X\"", state_path, "\"�ւ̃X�e�[�g�o�^�Ɏ��s���܂����B�p�X����̉\��������܂��B");
		return false;
	}
	//�e�̒T��
	size_t parent_state_data_idx = 0;
	for (int i = 0; i < (signed)id_hierarchy.size() - 1; ++i) {
		auto& cld_dat_idxes = state_data_list_[parent_state_data_idx].child_state_data_idxes;
		auto cld_dat_idx_map_it = cld_dat_idxes.find(id_hierarchy[i]);
		if (cld_dat_idx_map_it == cld_dat_idxes.end()) {
			std::string parent_state_path;
			for (int j = 0; j < i - 1; ++j) {
				parent_state_path = parent_state_path + id_hierarchy[j] + ".";
			}
			parent_state_path += id_hierarchy[i - 1];
			PE_LOG_ERROR("�X�e�[�g�p�X\"", state_path, "\"�ւ̃X�e�[�g�o�^�Ɏ��s���܂����B�X�e�[�g�p�X\"", parent_state_path, "\"�̎q��ID��\"", id_hierarchy[i], "\"�ł���X�e�[�g�͑��݂��܂���B");
			return false;
		} else {
			parent_state_data_idx = cld_dat_idx_map_it->second;
		}
	}
	//�e���w�肵�ēo�^
	return RegisterStateWithParentStateDataIdx_(creator, *(--id_hierarchy.end()), parent_state_data_idx);
}

bool CStateMachine::RegisterStateWithParentStateDataIdx_(const std::function<std::shared_ptr<StateBase>()>& creator, const std::string& state_id, size_t parent_state_data_idx) {
	StateData sd;
	sd.state_id = state_id;
	sd.creator = creator;
	sd.state_hierarchy_depth = state_data_list_[parent_state_data_idx].state_hierarchy_depth + 1;
	//�e�X�e�[�g�̃C���f�b�N�X��ݒ�
	sd.parent_state_data_idx = parent_state_data_idx;
	//�X�e�[�g�}�b�v�ł̑��݊m�F
	/*if (state_data_map_.find(state_id) != state_data_map_.end()) {
		PE_LOG_WARNING("���łɑ��݂���X�e�[�g\"", state_id, "\"���w�肳�ꂽ���ߒǉ����܂���ł����B");
		return true;
	}*/
	//�X�e�[�g�f�[�^���X�g�ɓo�^���ăC���f�b�N�X���擾
	state_data_list_.push_back(sd);
	int idx = state_data_list_.size() - 1;
	//�C���f�b�N�X��e�X�e�[�g�f�[�^�ɓo�^
	if (!std::get<bool>(state_data_list_[parent_state_data_idx].child_state_data_idxes.emplace(state_id, idx))) {
		PE_LOG_WARNING("���łɑ��݂���X�e�[�g\"", state_id, "\"���w�肳�ꂽ���ߏ㏑������܂����B");
	}
	//�C���f�b�N�X���X�e�[�g�f�[�^�}�b�v�ɓo�^
	//state_data_map_.emplace(state_id, idx);
	return true;
}

bool CStateMachine::StartStateMachine(const std::string& id) {
	return Transition(id);
}

void CStateMachine::StopStateMachine() {
	for (auto it = active_state_list_.rbegin(); it != active_state_list_.rend(); ++it) {
		(*it).second->ExitProcBase();
	}
	active_state_list_.clear();
}

bool CStateMachine::Transition(const std::string& state_id) {
	return Transition_(state_id);
}

bool CStateMachine::Transition_(const std::string& state_id) {
	std::list<std::string> ids;
	SeparateStatePathToStateIdHierarchy(ids, state_id);
	return ParentBaseTransition_(state_data_list_[0], ids);
}

bool CStateMachine::SendMessage(const std::string& message) {
	if (active_state_list_.empty()) {
		PE_LOG_ERROR("�X�e�[�g�}�V�����J�n����Ă��܂���B");
		return false;
	}
	int d = 0;
	for (auto&& stt : active_state_list_) {
		auto trans_id = stt.second->MessageHandlerBase(message);
		if (trans_id) {
			return MeBaseTransition_(state_data_list_[stt.first], *trans_id);
		}
		++d;
	}
	return true;
}

void CStateMachine::OnInitialized() {
	Super::OnInitialized();
	auto task = game_object().CreateAndAttachTask<TInstant>(TaskSlot::PreCollisionEarlyPhase);
	task->SetExcuteFunction([this] {Update_(); });
}

void CStateMachine::Update_() {
	int d = 0;
	for (auto&& stt : active_state_list_) {
		auto ts = stt.second->UpdateProcBase();
		if (ts) {
			MeBaseTransition_(state_data_list_[stt.first], *ts);
			break;
		}
		++d;
	}
}

#if defined(_MSC_VER) && _MSC_VER >= 1400 
#pragma warning(pop) 
#endif 