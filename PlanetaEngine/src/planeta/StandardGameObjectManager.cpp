#include "planeta/StandardGameObjectManager.hpp"
#include "GameObjectBase.hpp"
#include "StandardTaskManager.hpp"
#include "planeta/LogUtility.hpp"
#include "GameObjectFactory.hpp"
#include "IDebugManager.hpp"
#include "boost/algorithm/string.hpp"

namespace plnt {
	namespace private_ {
		StandardGameObjectManager::StandardGameObjectManager() :game_object_factory_(std::make_unique<private_::GameObjectFactory>()), _id_counter(0) {};
		StandardGameObjectManager::~StandardGameObjectManager() = default;

		void StandardGameObjectManager::Update() {
			RemoveProc_();
		}

		std::shared_ptr<GameObjectBase> StandardGameObjectManager::CreateAndSetUpGameObject_(const std::string& game_object_type_id) {
			auto go = game_object_factory_->GetNewGameObject(game_object_type_id, scene_internal_interface());
			return go;
		}

		std::shared_ptr<GameObjectBase> StandardGameObjectManager::CreateAndSetUpGameObject_(const std::vector<std::string>& com_type_list) {
			auto go = game_object_factory_->GetNewGameObject(com_type_list, scene_internal_interface());
			return go;
		}

		int StandardGameObjectManager::RegisterAndInitializeGameObject_(const std::shared_ptr<GameObjectBase>& go) {
			assert(go != nullptr);
			int id = _id_counter++;
			go->SetManagerConnection(std::make_unique<GameObjectManagerConnection>(*this, id));
			if (go->ProcessInitialization() == false) {
				PE_LOG_ERROR("ゲームオブジェクトの初期化に失敗しました。");
				return -1;
			}
			inactive_game_objects_.insert(std::make_pair(id, go));
			return id;
		}

		int StandardGameObjectManager::RegisterAndInitializeGameObject_(const std::shared_ptr<GameObjectBase>& go, const std::string& name) {
			assert(go != nullptr);
			int id = RegisterAndInitializeGameObject_(go);
			if (id < -1) { return -1; }
			name_id_map_.emplace(name, id);
			return id;
		}

		WeakPointer<IGameObject> StandardGameObjectManager::CreateGameObject(const std::string& game_object_def_file_id) {
			auto go = CreateAndSetUpGameObject_(game_object_def_file_id);
			if (go != nullptr && RegisterAndInitializeGameObject_(go) >= 0) {
				return go;
			} else {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(GameObject定義ファイルID:", game_object_def_file_id, ")");
				return nullptr;
			}
		}

		WeakPointer<IGameObject> StandardGameObjectManager::CreateGameObject(const std::string& game_object_def_file_id, const std::string& name) {
			auto go = CreateAndSetUpGameObject_(game_object_def_file_id);
			if (go != nullptr && RegisterAndInitializeGameObject_(go, name) >= 0) {
				return go;
			} else {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(GameObject定義ファイルID:", game_object_def_file_id, ")");
				return nullptr;
			}
		}

		WeakPointer<IGameObject> StandardGameObjectManager::CreateGameObjectWithComponentTypeIDList(const std::vector<std::string>& game_object_component_type_id_list) {
			auto go = CreateAndSetUpGameObject_(game_object_component_type_id_list);
			if (go != nullptr && RegisterAndInitializeGameObject_(go) >= 0) {
				return go;
			} else {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(コンポーネントの指定による作成。指定されたコンポーネントTypeIDは:", boost::algorithm::join(game_object_component_type_id_list, "."), ")");
				return nullptr;
			}
		}
		WeakPointer<IGameObject> StandardGameObjectManager::CreateGameObjectWithComponentTypeIDList(const std::vector<std::string>& game_object_component_type_id_list, const std::string& name) {
			auto go = CreateAndSetUpGameObject_(game_object_component_type_id_list);
			if (go != nullptr && RegisterAndInitializeGameObject_(go, name) >= 0) {
				return go;
			} else {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(コンポーネントの指定による作成。指定されたコンポーネントTypeIDは:", boost::algorithm::join(game_object_component_type_id_list, "."), ")");
				return nullptr;
			}
		}

		bool StandardGameObjectManager::ActivateGameObject(int id) {
			auto it = inactive_game_objects_.find(id);
			if (it == inactive_game_objects_.end()) { return false; }
			it->second->ProcessActivation();
			active_game_objects_.insert(*it);
			inactive_game_objects_.erase(it);
			return true;
		}

		bool StandardGameObjectManager::InActivateGameObject(int id) {
			auto it = active_game_objects_.find(id);
			if (it == active_game_objects_.end()) { return false; }
			it->second->ProcessInactivation();
			inactive_game_objects_.insert(*it);
			active_game_objects_.erase(it);
			return true;
		}

		bool StandardGameObjectManager::RemoveGameObject(int id) {
			auto it = active_game_objects_.find(id);
			if (it == active_game_objects_.end()) {
				it = inactive_game_objects_.find(id);
				if (it == inactive_game_objects_.end()) { return false; } else {
					it->second->ProcessDisposal();
					garbage_.push_back(it->second);
					inactive_game_objects_.erase(it);
					return true;
				}
			} else {
				it->second->ProcessInactivation(); //アクティブリストにあったゲームオブジェクトは、無効化処理を行ってから破棄する
				it->second->ProcessDisposal();
				garbage_.push_back(it->second);
				active_game_objects_.erase(it);
				return true;
			}
		}

		void StandardGameObjectManager::RemoveAllGameObjects() {
			for (auto& go : active_game_objects_) {
				go.second->ProcessInactivation();
				go.second->ProcessDisposal();
			}
			active_game_objects_.clear();
			for (auto& go : inactive_game_objects_) {
				go.second->ProcessDisposal();
			}
			inactive_game_objects_.clear();
		}

		bool StandardGameObjectManager::Initialize() {
			return true;
		}

		void StandardGameObjectManager::Finalize() {
			RemoveAllGameObjects();
		}

		void StandardGameObjectManager::RemoveProc_() {
			garbage_.clear();
		}

		void StandardGameObjectManager::DebugInformationAddHandle(IDebugInformationAdder& di_adder) {
			di_adder.AddLine("-----GameObjectManager-----");
			di_adder.AddLineV("活動中ゲームオブジェクト数:", active_game_objects_.size());
			di_adder.AddLineV("停止中ゲームオブジェクト数:", inactive_game_objects_.size());
		}

	}
}