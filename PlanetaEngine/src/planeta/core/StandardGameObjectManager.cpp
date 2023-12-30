#include "StandardGameObjectManager.hpp"
#include "game_object_base.hpp"
#include "StandardTaskManager.hpp"
#include "log_utility.hpp"
#include "game_object_factory.hpp"
#include "i_debug_manager.hpp"
#include "boost/algorithm/string.hpp"

namespace plnt {
	namespace private_ {
		StandardGameObjectManager::StandardGameObjectManager() : game_object_factory_(
			                                                         std::make_unique<private_::game_object_factory>()),
		                                                         _id_counter(0) { };
		StandardGameObjectManager::~StandardGameObjectManager() = default;

		void StandardGameObjectManager::update() { RemoveProc_(); }

		std::shared_ptr<game_object_base> StandardGameObjectManager::CreateAndSetUpGameObject_(
			const std::string &game_object_type_id) {
			auto go = game_object_factory_->get_new_game_object(game_object_type_id, scene_internal_interface());
			return go;
		}

		std::shared_ptr<game_object_base> StandardGameObjectManager::CreateAndSetUpGameObject_(
			const std::vector<std::string> &com_type_list) {
			auto go = game_object_factory_->get_new_game_object(com_type_list, scene_internal_interface());
			return go;
		}

		int StandardGameObjectManager::RegisterAndInitializeGameObject_(const std::shared_ptr<game_object_base> &go) {
			assert(go != nullptr);
			int id = _id_counter++;
			go->set_manager_connection(std::make_unique<game_object_manager_connection>(*this, id));
			if (go->process_initialization() == false) {
				PE_LOG_ERROR("ゲームオブジェクトの初期化に失敗しました。");
				return -1;
			}
			inactive_game_objects_.insert(std::make_pair(id, go));
			return id;
		}

		int StandardGameObjectManager::RegisterAndInitializeGameObject_(const std::shared_ptr<game_object_base> &go,
		                                                                const std::string &name) {
			assert(go != nullptr);
			int id = RegisterAndInitializeGameObject_(go);
			if (id < -1) { return -1; }
			name_id_map_.emplace(name, id);
			return id;
		}

		WeakPointer<i_game_object>
		StandardGameObjectManager::create_game_object(const std::string &game_object_def_file_id) {
			auto go = CreateAndSetUpGameObject_(game_object_def_file_id);
			if (go != nullptr && RegisterAndInitializeGameObject_(go) >= 0) { return go; } else {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(GameObject定義ファイルID:", game_object_def_file_id, ")");
				return nullptr;
			}
		}

		WeakPointer<i_game_object> StandardGameObjectManager::create_game_object(
			const std::string &game_object_def_file_id, const std::string &name) {
			auto go = CreateAndSetUpGameObject_(game_object_def_file_id);
			if (go != nullptr && RegisterAndInitializeGameObject_(go, name) >= 0) { return go; } else {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(GameObject定義ファイルID:", game_object_def_file_id, ")");
				return nullptr;
			}
		}

		WeakPointer<i_game_object> StandardGameObjectManager::create_game_object_with_component_type_id_list(
			const std::vector<std::string> &game_object_component_type_id_list) {
			auto go = CreateAndSetUpGameObject_(game_object_component_type_id_list);
			if (go != nullptr && RegisterAndInitializeGameObject_(go) >= 0) { return go; } else {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(コンポーネントの指定による作成。指定されたコンポーネントTypeIDは:",
				             boost::algorithm::join(game_object_component_type_id_list, "."), ")");
				return nullptr;
			}
		}

		WeakPointer<i_game_object> StandardGameObjectManager::create_game_object_with_component_type_id_list(
			const std::vector<std::string> &game_object_component_type_id_list, const std::string &name) {
			auto go = CreateAndSetUpGameObject_(game_object_component_type_id_list);
			if (go != nullptr && RegisterAndInitializeGameObject_(go, name) >= 0) { return go; } else {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(コンポーネントの指定による作成。指定されたコンポーネントTypeIDは:",
				             boost::algorithm::join(game_object_component_type_id_list, "."), ")");
				return nullptr;
			}
		}

		bool StandardGameObjectManager::activate_game_object(int id) {
			auto it = inactive_game_objects_.find(id);
			if (it == inactive_game_objects_.end()) { return false; }
			it->second->process_activation();
			active_game_objects_.insert(*it);
			inactive_game_objects_.erase(it);
			return true;
		}

		bool StandardGameObjectManager::inactivate_game_object(int id) {
			auto it = active_game_objects_.find(id);
			if (it == active_game_objects_.end()) { return false; }
			it->second->process_inactivation();
			inactive_game_objects_.insert(*it);
			active_game_objects_.erase(it);
			return true;
		}

		bool StandardGameObjectManager::remove_game_object(int id) {
			auto it = active_game_objects_.find(id);
			if (it == active_game_objects_.end()) {
				it = inactive_game_objects_.find(id);
				if (it == inactive_game_objects_.end()) { return false; } else {
					it->second->process_disposal();
					garbage_.push_back(it->second);
					inactive_game_objects_.erase(it);
					return true;
				}
			} else {
				it->second->process_inactivation(); //アクティブリストにあったゲームオブジェクトは、無効化処理を行ってから破棄する
				it->second->process_disposal();
				garbage_.push_back(it->second);
				active_game_objects_.erase(it);
				return true;
			}
		}

		void StandardGameObjectManager::remove_all_game_objects() {
			for (auto &go : active_game_objects_) {
				go.second->process_inactivation();
				go.second->process_disposal();
			}
			active_game_objects_.clear();
			for (auto &go : inactive_game_objects_) { go.second->process_disposal(); }
			inactive_game_objects_.clear();
		}

		bool StandardGameObjectManager::initialize() { return true; }

		void StandardGameObjectManager::finalize() { remove_all_game_objects(); }

		void StandardGameObjectManager::RemoveProc_() { garbage_.clear(); }

		void StandardGameObjectManager::debug_information_add_handle(i_debug_information_adder &di_adder) {
			di_adder.add_line("-----GameObjectManager-----");
			di_adder.add_line_v("活動中ゲームオブジェクト数:", active_game_objects_.size());
			di_adder.add_line_v("停止中ゲームオブジェクト数:", inactive_game_objects_.size());
		}
	}
}
