#include "GameObjectManager.h"
#include "GameObjectBase.h"
#include "TaskManager.h"
#include "SystemLog.h"
#include "SceneData.h"
#include "Reflection.h"
#include "PrefixUtility.h"

namespace planeta {
	GameObjectManager::GameObjectManager() :_id_counter(0) {};
	GameObjectManager::~GameObjectManager() = default;

	void GameObjectManager::Update() {
		RemoveProc_();
	}

	int GameObjectManager::RegisterAndSetUpGameObject_(const std::shared_ptr<GameObjectBase>& go) {
		go->SetSceneData(scene_data_);
		int id = _id_counter++;
		go->SetManagerConnection(std::make_unique<GameObjectManagerConnection>(*this, id));
		if (go->ProcessInitialization() == false) {
			PE_LOG_ERROR("ゲームオブジェクトの初期化に失敗しました。");
			return -1;
		}
		inactive_game_objects_.insert(std::make_pair(id, go));
		return id;
	}

	int GameObjectManager::RegisterAndSetUpGameObject_(const std::shared_ptr<GameObjectBase>& go, const std::string& name) {
		int id = RegisterAndSetUpGameObject_(go);
		if (id < -1) { return -1; }
		name_id_map_.emplace(name, id);
		return id;
	}

	void GameObjectManager::TakeOver(const GameObjectManager& gom) {
		for (auto& go : gom.active_game_objects_) {
			active_game_objects_.insert(std::move(go));
		}
	}

	util::WeakPointer<IGameObject> GameObjectManager::CreateGameObject(const std::string& game_object_create_id) {
		auto go = CreateGameObjectByID_(game_object_create_id);
			if (go == nullptr) {
				PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。");
				return nullptr;
			}
		if (RegisterAndSetUpGameObject_(go) >= 0) {
			return go;
		} else { return nullptr; }

	}
	util::WeakPointer<IGameObject> GameObjectManager::CreateGameObject(const std::string& game_object_create_id, const std::string& name) {
		auto go = CreateGameObjectByID_(game_object_create_id);
		if (go == nullptr) {
			PE_LOG_ERROR("ゲームオブジェクトの作成に失敗しました。");
			return nullptr;
		}
		if (RegisterAndSetUpGameObject_(go, name) >= 0) {
			return go;
		} else { return nullptr; }

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

	std::shared_ptr<GameObjectBase > GameObjectManager::CreateGameObjectByID_(const std::string& id) {
		//IDにプレフィックスをつけたゲームオブジェクトを作成。
		return Reflection::CreateObjectByID<GameObjectBase>(core::AddPrefix(id, core::ObjectCategory::GameObject));
	}

}