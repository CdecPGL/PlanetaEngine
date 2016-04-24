#include "GameObjectManager.h"
#include "GameObjectBase.h"
#include "TaskManager.h"
#include "SceneAccessorForGameObject.h"
#include "SystemLog.h"
#include "SceneDataForGameObject.h"
#include "SceneData.h"
#include "GameObjectFactory.h"

namespace planeta_engine {
	GameObjectManager::GameObjectManager() :_id_counter(0), factory_(GameObjectFactory::GetInstance()) {};
	GameObjectManager::~GameObjectManager() = default;

	bool GameObjectManager::Process() {
		RemoveProc_();
		return true;
	}

	int GameObjectManager::RegisterGameObject(const std::shared_ptr<GameObjectBase>& go) {
		go->SetSceneAccessor(scene_accessor_);
		int id = _id_counter++;
		go->SetManagerConnection(std::make_unique<GameObjectManagerConnection>(*this, id));
		inactive_game_objects_.insert(std::make_pair(id, go));
		return id;
	}

	int GameObjectManager::RegisterGameObject(const std::shared_ptr<GameObjectBase>& go, const std::string& name) {
		int id = RegisterGameObject(go);
		if (id < -1) { return -1; }
		name_id_map_.emplace(name, id);
		return id;
	}

	void GameObjectManager::TakeOver(const GameObjectManager& gom) {
		for (auto& go : gom.active_game_objects_) {
			active_game_objects_.insert(std::move(go));
		}
	}

	utility::WeakPointer<IGameObject> GameObjectManager::CreateGameObject(const std::string& game_object_create_id) {
		auto go = factory_.CreateGameObject(game_object_create_id);
			if (go == nullptr) {
				debug::SystemLog::instance().LogError("ゲームオブジェクトの作成に失敗しました。", __FUNCTION__);
				return nullptr;
			}
		if (RegisterGameObject(go) >= 0) {
			return go;
		} else { return nullptr; }

	}
	utility::WeakPointer<IGameObject> GameObjectManager::CreateGameObject(const std::string& game_object_create_id, const std::string& name) {
		auto go = factory_.CreateGameObject(game_object_create_id);
		if (go == nullptr) {
			debug::SystemLog::instance().LogError("ゲームオブジェクトの作成に失敗しました。", __FUNCTION__);
			return nullptr;
		}
		if (RegisterGameObject(go, name) >= 0) {
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

	void GameObjectManager::SetSceneInterface(core::ScenePublicInterface& spi) {
		scene_accessor_ = std::make_shared<SceneAccessorForGameObject>(spi);
	}

	void GameObjectManager::Update() {
		for (auto it = active_game_objects_.begin(); it != active_game_objects_.end();) {
			try {
				(it++)->second->Update();
			} catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("GameObject::Updateで無効なWeakPointerが参照されました。問題の発生したGameObjectはリストから除外されます。") + e.what(), __FUNCTION__);
				active_game_objects_.erase(it);
				assert(false);
				break;
			}
		}
	}

	void GameObjectManager::RemoveProc_() {
		garbage_.clear();
	}

	void GameObjectManager::SetSceneData(const core::SceneData& scene_data) {
		scene_data_ = std::make_unique<core::SceneDataForGameObject>();
		scene_data_->collision_detect_process = scene_data.collision_detect_process;
		scene_data_->draw_component_process_registrator = scene_data.draw_component_process_registrator;
	}
}