#include "GameObjectManager.h"
#include "GameObject.h"
#include "GameProcessManager.h"
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
				debug::SystemLog::instance().LogError("GameObject�̏������Ɏ��s���܂����B", "GameObjectManager::Resist");
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
				debug::SystemLog::instance().LogError("�����ȃQ�[���I�u�W�F�N�g�ł��B", "GameObjectManager::CreateGameObject");
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
				debug::SystemLog::instance().LogError("�����ȃQ�[���I�u�W�F�N�g�ł��B", "GameObjectManager::CreateGameObject");
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
			auto it = inactive_game_objects_.find(id);
			if (it == inactive_game_objects_.end()) { return false; }
			it->second->_Activated();
			active_game_objects_.insert(*it);
			inactive_game_objects_.erase(it);
			return true;
		}

		bool GameObjectManager::InActivate(int id)
		{			
			auto it = active_game_objects_.find(id);
			if (it == active_game_objects_.end()) { return false; }
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
				it->second->_InActivated(); //�A�N�e�B�u���X�g�ɂ������Q�[���I�u�W�F�N�g�́A�������������s���Ă���j������
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

		GameObjectManager::GameObjectManager() :_id_counter(0){};

		void GameObjectManager::SetScene(core::ScenePublicInterface& spi) {
			scene_accessor_ = std::make_shared<core::SceneAccessorForGameObject>(spi);
		}

		void GameObjectManager::Update() {
			for (auto it = active_game_objects_.begin(); it != active_game_objects_.end();++it) {
				try {
					it->second->Update();
				} catch (utility::NullWeakPointerException& e) {
					debug::SystemLog::instance().LogError(std::string("GameObject::Update�Ŗ�����WeakPointer���Q�Ƃ���܂����B���̔�������GameObject�̓��X�g���珜�O����܂��B") + e.what(), __FUNCTION__);
					active_game_objects_.erase(it);
					assert(false);
					break;
				}
			}
		}
	}
}