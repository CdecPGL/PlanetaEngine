#include "GameObjectManager.h"
#include "GameObjectBase.h"
#include "TaskManager.h"
#include "SystemLog.h"
#include "SceneData.h"
#include "GameObjectTemplateHolder.h"

namespace planeta {
	GameObjectManager::GameObjectManager() :game_object_template_holder_(std::make_unique<core::GameObjectTemplateHolder>()), _id_counter(0) {};
	GameObjectManager::~GameObjectManager() = default;

	void GameObjectManager::Update() {
		RemoveProc_();
	}

	std::shared_ptr<GameObjectBase> GameObjectManager::CreateAndSetUpGameObject_(const std::string& game_object_type_id, const std::string& resource_id) {
		auto go = game_object_template_holder_->GetNewGameObject(game_object_type_id, resource_id, scene_data_);
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

	util::WeakPointer<IGameObject> GameObjectManager::CreateGameObject(const std::string& game_object_type_id, const std::string& file_id) {
		auto go = CreateAndSetUpGameObject_(game_object_type_id, file_id);
		if (go != nullptr && RegisterAndInitializeGameObject_(go)>=0) {
			return go;
		} else {
			PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(GameObjectTypeID:", game_object_type_id, ", FileID:", file_id, ")");
			return nullptr;
		}
	}

	util::WeakPointer<IGameObject> GameObjectManager::CreateGameObject(const std::string& game_object_type_id, const std::string& file_id, const std::string& name) {
		auto go = CreateAndSetUpGameObject_(game_object_type_id, file_id);
		if (go != nullptr && RegisterAndInitializeGameObject_(go, name)>=0) {
			return go;
		} else {
			PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。(GameObjectTypeID:", game_object_type_id, ", FileID:", file_id, ")");
			return nullptr;
		}
	}

	util::WeakPointer<IGameObject> GameObjectManager::CreateDefaultGameObject(const std::string& game_object_create_id) {
		return CreateGameObject(game_object_create_id, {});
	}
	util::WeakPointer<IGameObject> GameObjectManager::CreateDefaultGameObject(const std::string& game_object_create_id, const std::string& name) {
		return CreateGameObject(game_object_create_id, {}, name);

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

	void GameObjectManager::SetSceneData(const util::WeakPointer<core::SceneData>& scene_data) {
		scene_data_ = scene_data;
	}
}