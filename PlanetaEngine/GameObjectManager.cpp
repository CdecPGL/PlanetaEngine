#include "GameObjectManager.h"
#include "GameObject.h"
#include "GameProcessManager.h"
#include "GameObjectUpdateProcess.h"
#include "SceneAccessorForGameObject.h"
#include "SystemLog.h"

namespace planeta_engine{
	namespace game{
		bool GameObjectManager::Process()
		{
			return true;
		}

		int GameObjectManager::Resist(const std::shared_ptr<GameObject>& go)
		{
			go->SetSceneAccessor(scene_accessor_);
			int id = _id_counter++;
			if (go->_Initialize(std::make_unique<GameObjectResisterConnection>(*this, id)) == false) {
				debug::SystemLog::instance().LogError("GameObjectの初期化に失敗しました。", "GameObjectManager::Resist");
				return -1;
			}
			inactive_game_objects_.insert(std::make_pair(id, go));
			return id;
		}

		int GameObjectManager::Resist(const std::shared_ptr<GameObject>& go, const std::string& name)
		{
			int id = Resist(go);
			if (id < -1) { return -1; }
			name_id_map_.emplace(name, id);
			return id;
		}

		void GameObjectManager::TakeOver(const GameObjectManager& gom)
		{
			for (auto& go : gom.active_game_objects_){
				active_game_objects_.insert(std::move(go));
			}
		}

		utility::WeakPointer<IGameObjectAccessor> GameObjectManager::CreateGameObject(GameObjectSetUpper& game_object_setupper)
		{
			auto go = GameObject::Create(game_object_setupper);
			if (go == nullptr) {
				debug::SystemLog::instance().LogError("無効なゲームオブジェクトです。", "GameObjectManager::CreateGameObject");
				return nullptr;
			}
			if (Resist(go) >= 0) {
				return go;
			}
			else { return nullptr; }
			
		}
		utility::WeakPointer<IGameObjectAccessor> GameObjectManager::CreateGameObject(GameObjectSetUpper& game_object_setupper,const std::string& name)
		{
			auto go = GameObject::Create(game_object_setupper);
			if (go == nullptr) {
				debug::SystemLog::instance().LogError("無効なゲームオブジェクトです。", "GameObjectManager::CreateGameObject");
				return nullptr;
			}
			if (Resist(go, name) >= 0) {
				return go;
			}
			else { return nullptr; }
			
		}

		utility::WeakPointer<IGameObjectAccessor> GameObjectManager::CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper)
		{
			auto go = CreateGameObject(game_object_setupper);
			if (go == nullptr) { return nullptr; }
			go->Activate();
			return go;
		}

		utility::WeakPointer<IGameObjectAccessor> GameObjectManager::CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name)
		{
			auto go = CreateGameObject(game_object_setupper, name);
			if (go == nullptr) { return nullptr; }
			go->Activate();
			return go;
		}

		bool GameObjectManager::Activate(int id)
		{
			if (!game_object_update_process_) { //ゲームオブジェクト更新プロセスが取得できてなかったらする。それでもできなかったら失敗
				game_object_update_process_ = scene_accessor_->game_process_manager().GetSystemProcess<system_processes::GameObjectUpdateProcess>();
				if (!game_object_update_process_) { return false; }
			}
			auto it = inactive_game_objects_.find(id);
			if (it == inactive_game_objects_.end()) { return false; }
			it->second->_Activated();
			active_game_objects_.insert(*it);
			game_object_update_process_->Resist(id, it->second); //ゲームオブジェクト更新プロセスに登録
			inactive_game_objects_.erase(it);
			return true;
		}

		bool GameObjectManager::InActivate(int id)
		{			
			auto it = active_game_objects_.find(id);
			if (it == active_game_objects_.end()) { return false; }
			RemoveFromUpdateProcess_(id); //アップデートプロセスから削除
			it->second->_InActivated();
			inactive_game_objects_.insert(*it);
			active_game_objects_.erase(it);
			return true;
		}

		bool GameObjectManager::Remove(int id)
		{
			auto it = active_game_objects_.find(id);
			if (it == active_game_objects_.end()) {
				it = inactive_game_objects_.find(id);
				if (it==inactive_game_objects_.end()) { return false; }
				else {
					it->second->_Finalize();
					inactive_game_objects_.erase(it);
					return true;
				}
			}
			else {
				RemoveFromUpdateProcess_(id); //アップデートプロセスから削除
				it->second->_InActivated(); //アクティブリストにあったゲームオブジェクトは、無効化処理を行ってから破棄する
				it->second->_Finalize();
				active_game_objects_.erase(it);
				return true;
			}
		}

		void GameObjectManager::RemoveAllGameObjects()
		{
			for (auto& go : active_game_objects_) {
				go.second->_InActivated();
				go.second->_Finalize();
			}
			active_game_objects_.clear();
			for (auto& go : inactive_game_objects_) {
				go.second->_Finalize();
			}
			inactive_game_objects_.clear();
		}

		bool GameObjectManager::Initialize()
		{
			return true;
		}

		bool GameObjectManager::Finalize()
		{
			RemoveAllGameObjects();
			return true;
		}

		bool GameObjectManager::RemoveFromUpdateProcess_(int id)
		{
			if (!game_object_update_process_) { //ゲームオブジェクト更新プロセスが取得できてなかったらする。それでもできなかったら失敗
				game_object_update_process_ = scene_accessor_->game_process_manager().GetSystemProcess<system_processes::GameObjectUpdateProcess>();
				if (!game_object_update_process_) { return false; }
			}
			return game_object_update_process_->Remove(id); //ゲームオブジェクト更新プロセスから登録解除
		}

		GameObjectManager::GameObjectManager(core::ScenePublicInterface& spi) :_id_counter(0) ,scene_accessor_(std::make_shared<core::SceneAccessorForGameObject>(spi)){};
	}
}