#include "GameObjectManager.h"
#include "GameObjectBase.h"
#include "TaskManager.h"
#include "SystemLog.h"
#include "SceneData.h"
#include "GameObjectFactory.h"
#include "boost/algorithm/string.hpp"

namespace planeta {
	namespace private_ {
		GameObjectManager::GameObjectManager() :game_object_factory_(std::make_unique<private_::GameObjectFactory>()), _id_counter(0) {};
		GameObjectManager::~GameObjectManager() = default;

		void GameObjectManager::Update() {
			RemoveProc_();
		}

		std::shared_ptr<GameObjectBase> GameObjectManager::CreateAndSetUpGameObject_(const std::string& game_object_type_id) {
			auto go = game_object_factory_->GetNewGameObject(game_object_type_id, scene_data_);
			return go;
		}

		std::shared_ptr<GameObjectBase> GameObjectManager::CreateAndSetUpGameObject_(const std::vector<std::string>& com_type_list) {
			auto go = game_object_factory_->GetNewGameObject(com_type_list, scene_data_);
			return go;
		}

		int GameObjectManager::RegisterAndInitializeGameObject_(const std::shared_ptr<GameObjectBase>& go) {
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

		int GameObjectManager::RegisterAndInitializeGameObject_(const std::shared_ptr<GameObjectBase>& go, const std::string& name) {
			assert(go != nullptr);
			int id = RegisterAndInitializeGameObject_(go);
			if (id < -1) { return -1; }
			name_id_map_.emplace(name, id);
			return id;
		}

		WeakPointer<IGameObject> GameObjectManager::CreateGameObject(const std::string& game_object_def_file_id) {
			auto go = CreateAndSetUpGameObject_(game_object_def_file_id);
			if (go != nullptr && RegisterAndInitializeGameObject_(go) >= 0) {
				return go;
			} else {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(GameObject定義ファイルID:", game_object_def_file_id, ")");
				return nullptr;
			}
		}

		WeakPointer<IGameObject> GameObjectManager::CreateGameObject(const std::string& game_object_def_file_id, const std::string& name) {
			auto go = CreateAndSetUpGameObject_(game_object_def_file_id);
			if (go != nullptr && RegisterAndInitializeGameObject_(go, name) >= 0) {
				return go;
			} else {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(GameObject定義ファイルID:", game_object_def_file_id, ")");
				return nullptr;
			}
		}

		WeakPointer<IGameObject> GameObjectManager::CreateGameObjectWithComponentTypeIDList(const std::vector<std::string>& game_object_component_type_id_list) {
			auto go = CreateAndSetUpGameObject_(game_object_component_type_id_list);
			if (go != nullptr && RegisterAndInitializeGameObject_(go) >= 0) {
				return go;
			} else {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(コンポーネントの指定による作成。指定されたコンポーネントTypeIDは:", boost::algorithm::join(game_object_component_type_id_list, "."), ")");
				return nullptr;
			}
		}
		WeakPointer<IGameObject> GameObjectManager::CreateGameObjectWithComponentTypeIDList(const std::vector<std::string>& game_object_component_type_id_list, const std::string& name) {
			auto go = CreateAndSetUpGameObject_(game_object_component_type_id_list);
			if (go != nullptr && RegisterAndInitializeGameObject_(go, name) >= 0) {
				return go;
			} else {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(コンポーネントの指定による作成。指定されたコンポーネントTypeIDは:", boost::algorithm::join(game_object_component_type_id_list, "."), ")");
				return nullptr;
			}
		}

		bool GameObjectManager::ActivateGameObject(int id) {
			auto it = inactive_game_objects_.find(id);
			if (it == inactive_game_objects_.end()) { return false; }
			it->second->ProcessActivation();
			active_game_objects_.insert(*it);
			inactive_game_objects_.erase(it);
			return true;
		}

		bool GameObjectManager::InActivateGameObject(int id) {
			auto it = active_game_objects_.find(id);
			if (it == active_game_objects_.end()) { return false; }
			it->second->ProcessInactivation();
			inactive_game_objects_.insert(*it);
			active_game_objects_.erase(it);
			return true;
		}

		bool GameObjectManager::RemoveGameObject(int id) {
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

		void GameObjectManager::RemoveAllGameObjects() {
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

		bool GameObjectManager::Initialize() {
			return true;
		}

		void GameObjectManager::Finalize() {
			RemoveAllGameObjects();
		}

		void GameObjectManager::RemoveProc_() {
			garbage_.clear();
		}

		void GameObjectManager::SetSceneData(const WeakPointer<private_::SceneData>& scene_data) {
			scene_data_ = scene_data;
		}
	}
}