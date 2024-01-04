#include <cassert>

#include "boost/algorithm/string.hpp"

#include "game.hpp"
#include "game_object_base.hpp"

#include <ranges>

#include "game_object_component.hpp"
#include "game_object_component_setup_data.hpp"
#include "log_utility.hpp"
#include "task.hpp"
#include "go_component_adder.hpp"
#include "go_component_getter.hpp"
#include "..\buildin\r_ptree.hpp"
#include "i_resource_manager.hpp"
#include "planeta/reflection/reflection.hpp"
#include "i_scene_internal.hpp"

namespace plnt {
	namespace {
		std::vector<std::shared_ptr<const boost::property_tree::ptree>> get_com_def_ptrees(
			const boost::property_tree::ptree &pt) {
			std::vector<std::shared_ptr<const boost::property_tree::ptree>> out;
			//文字列として読み込んでみる
			//文字列が空でなかったらリソースIDとみなしてPtreeリソースを読み込む
			if (auto res_id = pt.get_value<std::string>(); !res_id.empty()) {
				//指定されたリソースがない
				if (const auto res = game::instance().resource_manager()->get_resource_by_id<r_ptree>(res_id); res ==
					nullptr) {
					PE_LOG_ERROR("ゲームオブジェクトコンポーネントのファイル定義読み込みに失敗しました。指定されたPtreeリソース\"", res_id, "\"を読み込めませんでした。");
				} else { out.push_back(res->get_ptree()); }
			} else {
				//文字列でなかったら、オブジェクトか配列
				if (pt.empty() || !pt.ordered_begin()->first.empty()) {
					//要素がないか、最初の要素にキーが存在したらオブジェクト。オブジェクトならそれがGOCDefinition
					out.push_back(std::make_shared<const boost::property_tree::ptree>(pt));
				} else {
					//配列にはGOC定義か、そのリソースIDが含まれる。
					for (const auto &val : pt | std::views::values) {
						if (auto res_id2 = val.get_value<std::string>(); !res_id2.empty()) {
							//要素が文字列だったら、そのIDのリソースを読み込み
							//指定されたリソースがない
							if (auto res = game::instance().resource_manager()->get_resource_by_id<r_ptree>(res_id2); res ==
								nullptr) {
								PE_LOG_ERROR("ゲームオブジェクトコンポーネントのファイル定義読み込みに失敗しました。指定されたPtreeリソース\"", res_id2,
								             "\"を読み込めませんでした。");
							} else { out.push_back(res->get_ptree()); }
						} else {
							//そうじゃなかったらそれがGOC定義の一つ
							out.push_back(std::make_shared<const boost::property_tree::ptree>(val));
						}
					}
				}
			}
			return out;
		}
	}

	namespace private_ {
		game_object_base::game_object_base() = default;

		game_object_base::~game_object_base() = default;

		weak_pointer<i_game_object> game_object_base::get_pointer() {
			assert(shared_from_this() != nullptr);
			return shared_from_this();
		}

		void game_object_base::activate() {
			if (!manager_connection_->request_activation()) { PE_LOG_FATAL("ゲームオブジェクトの有効化に失敗しました。"); }
		}

		void game_object_base::inactivate() {
			if (!manager_connection_->request_inactivation()) { PE_LOG_FATAL("ゲームオブジェクトの無効化に失敗しました。"); }
		}

		void game_object_base::dispose() { manager_connection_->request_disposal(); }

		bool game_object_base::process_initialization() {
			state_ = game_object_state::initializing;

			const go_component_getter com_getter(component_holder_);

			decltype(auto) com_ary = component_holder_.component_array();

			for (auto &&com : com_ary) {
				if (!com->get_other_components_proc(com_getter)) {
					// NOLINTNEXTLINE(clang-diagnostic-potentially-evaluated-expression)
					PE_LOG_ERROR("GameObjectComponent\"型:", typeid(*com).name(), "\"でほかのオブジェクトを取得する処理に失敗しました。");
					state_ = game_object_state::invalid;
					return false;
				}
			}

			for (auto &&com : com_ary) { com->initialize(); }
			state_ = game_object_state::inactive;
			return true;
		}

		bool game_object_base::process_activation() {
			state_ = game_object_state::activating;
			//コンポーネントの有効化
			for (decltype(auto) com_ary = component_holder_.component_array(); auto &&com : com_ary) {
				com->activate();
			}
			//有効化イベント
			activated();
			//アタッチされたタスクの再開
			check_and_apply_process_to_attached_task([](task &t)-> bool { return t.resume(); });

			state_ = game_object_state::active;
			return true;
		}

		bool game_object_base::process_inactivation() {
			state_ = game_object_state::inactivating;
			//アタッチされたタスクの停止
			check_and_apply_process_to_attached_task([](task &t)-> bool { return t.pause(); });
			//無効化イベント
			inactivated();
			//コンポーネントの無効化
			for (decltype(auto) com_ary = component_holder_.component_array(); auto &&com : com_ary) {
				com->inactivate();
			}

			state_ = game_object_state::inactive;
			return true;
		}

		bool game_object_base::process_disposal() {
			state_ = game_object_state::invalid;
			//アタッチされたタスクの破棄
			check_and_apply_process_to_attached_task([](task &t)-> bool {
				t.dispose();
				return true;
			});
			attached_tasks_.clear();
			//破棄時イベント
			disposed();
			//コンポーネントの終了処理
			for (auto &&com : component_holder_.component_array()) { com->finalize(); }
			return true;
		}

		void game_object_base::set_manager_connection(std::unique_ptr<game_object_manager_connection> &&mgr_conn) {
			manager_connection_ = std::move(mgr_conn);
		}

		void game_object_base::set_scene_internal_interface(const weak_pointer<i_scene_internal> &i_scene) {
			scene_internal_interface_ = i_scene;
		}

		void game_object_base::set_scene_and_go_data_to_component(game_object_component &com) {
			const game_object_component_set_up_data rd{this, scene_internal_interface_};
			com.set_scene_and_holder_go_data(rd);
		}

		bool game_object_base::add_components_from_type_id_list(const std::vector<std::string> &com_type_id_list,
		                                                      std::vector<std::shared_ptr<game_object_component>> &
		                                                      added_coms) {
			go_component_adder com_adder{component_holder_};
			for (auto &&com_type_id : com_type_id_list) {
				if (auto com = com_adder.create_and_add_component(com_type_id)) { added_coms.push_back(com); } else {
					PE_LOG_ERROR("ゲームオブジェクトコンポーネント(GOCTypeID:", com_type_id, ")の追加に失敗しました。");
					return false;
				}
			}
			return true;
		}

		bool game_object_base::set_data_to_components_from_ptree(
			const std::vector<std::shared_ptr<game_object_component>> &coms,
			const std::vector<std::shared_ptr<const boost::property_tree::ptree>> &pts) {
			assert(coms.size() == pts.size());
			for (size_t i = 0; i < coms.size(); ++i) {
				//Ptreeからリフレクションシステムを用いて読み込み
				try { coms[i]->reflective_load_from_ptree(*pts[i]); } catch (reflection::reflection_error &e) {
					// NOLINTNEXTLINE(clang-diagnostic-potentially-evaluated-expression)
					PE_LOG_ERROR("ゲームオブジェクトコンポーネント(\"型:", typeid(*coms[i]).name(),
					             "\")のファイル定義読み込みに失敗しました。エラーが発生したか、コンポーネントがファイル定義読み込みに対応していない可能性があります。ファイル定義読み込み関数を継承しているか確認してください。(",
					             e.what(), ")");
					return false;
				}
			}
			return true;
		}

		std::shared_ptr<game_object_component> game_object_base::get_component_by_type_info(
			const std::type_info &ti, const std::function<bool(game_object_component *goc)> &type_checker) const {
			return component_holder_.get_component_by_type_info(ti, type_checker);
		}

		//std::vector<std::shared_ptr<GameObjectComponent>> GameObjectBase::GetAllComponentsByTypeInfo(const std::type_info& ti, const std::function<bool(GameObjectComponent* goc)>& type_checker) const {
		//	return std::move(component_holder_.GetAllComponentsByTypeInfo(ti, type_checker));
		//}

		void game_object_base::set_up_attached_task(const weak_pointer<task> &task) {
			if (state_ == game_object_state::active || state_ == game_object_state::activating) { task->resume(); }
			attached_tasks_.push_back(task);
		}

		bool game_object_base::process_cloning(const std::shared_ptr<game_object_base> &dst) {
			//シーンデータのセット
			scene_internal_interface_ = dst->scene_internal_interface_;
			//コンポーネントのクローン
			dst->component_holder_.clone_to_other_holder(component_holder_);
			//コンポーネントへシーンとGOデータをセット
			set_scene_and_go_data_to_components();
			return true;
		}

		bool game_object_base::add_and_set_up_components(const boost::property_tree::ptree &pt) {
			//コンポーネント型IDと対応するPtreeの読み込み
			std::vector<std::string> com_type_ids;
			std::vector<std::vector<std::shared_ptr<const boost::property_tree::ptree>>> pts_each_com_tid;
			for (auto &&com_defs : pt) {
				auto com_def_pts = get_com_def_ptrees(com_defs.second);
				pts_each_com_tid.push_back(std::move(com_def_pts));
				com_type_ids.push_back(com_defs.first);
			}
			assert(com_type_ids.size() == pts_each_com_tid.size());
			//コンポーネントの追加
			std::vector<std::vector<std::shared_ptr<game_object_component>>> coms_each_com_tid;
			for (size_t i = 0; i < com_type_ids.size(); ++i) {
				std::vector<std::shared_ptr<game_object_component>> coms;
				std::vector<std::string> com_tid_list;
				com_tid_list.resize(pts_each_com_tid[i].size(), com_type_ids[i]);
				if (!add_components_from_type_id_list(com_tid_list, coms)) {
					PE_LOG_ERROR("コンポーネントの作成に失敗しました。");
					return false;
				}
				coms_each_com_tid.push_back(std::move(coms));
			}
			assert(com_type_ids.size() == coms_each_com_tid.size());
			//コンポーネントのセットアップ
			set_scene_and_go_data_to_components();
			//コンポーネントデータをptreeから読み込み
			for (size_t i = 0; i < com_type_ids.size(); ++i) {
				if (!set_data_to_components_from_ptree(coms_each_com_tid[i], pts_each_com_tid[i])) {
					PE_LOG_ERROR("PtreeからのComponentのデータセットに失敗しました。");
					return false;
				}
			}
			return true;
		}

		bool game_object_base::add_and_set_up_components(const std::vector<std::string> &com_type_id_list) {
			//コンポーネントの追加
			if (std::vector<std::shared_ptr<game_object_component>> coms; !
				add_components_from_type_id_list(com_type_id_list, coms)) {
				PE_LOG_ERROR("コンポーネントの作成に失敗しました。");
				return false;
			}
			//コンポーネントのセットアップ
			set_scene_and_go_data_to_components();
			return true;
		}

		void game_object_base::set_scene_and_go_data_to_components() {
			decltype(auto) com_ary = component_holder_.component_array();
			for (auto &&com : com_ary) { set_scene_and_go_data_to_component(*com); }
		}

		bool game_object_base::check_and_apply_process_to_attached_task(const std::function<bool(task &)> &proc) {
			bool scc = true;
			for (auto it = attached_tasks_.begin(); it != attached_tasks_.end();) {
				if (auto pre = it++; !(*pre)) { attached_tasks_.erase(pre); } else { scc &= proc(**pre); }
			}
			return scc;
		}

		game_object_state game_object_base::state() const { return state_; }

		i_scene &game_object_base::scene() { return *scene_internal_interface_; }
	}
}
