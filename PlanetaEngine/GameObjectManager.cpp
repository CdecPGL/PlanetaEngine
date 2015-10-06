#include "GameObjectManager.h"
#include "GameObject.h"
#include "GameProcessManager.h"
#include "GameObjectUpdateProcess.h"
#include "ISceneAccessForGameObject.h"
#include "SystemLog.h"

namespace planeta_engine{
	namespace game{
		bool GameObjectManager::Process()
		{
			return true;
		}

		int GameObjectManager::Resist(const std::shared_ptr<GameObject>& go)
		{
			go->SetSceneAccessor(_scene);
			int id = _id_counter++;
			if (go->_Initialize(id) == false) {
				debug::SystemLog::instance().LogError("GameObject�̏������Ɏ��s���܂����B", "GameObjectManager::Resist");
				return -1;
			}
			_inactive_game_objects.insert(std::make_pair(id, go));
			return id;
		}

		int GameObjectManager::Resist(const std::shared_ptr<GameObject>& go, const std::string& name)
		{
			int id = Resist(go);
			if (id < -1) { return -1; }
			_name_id_map.emplace(name, id);
			return id;
		}

		void GameObjectManager::TakeOver(const GameObjectManager& gom)
		{
			for (auto& go : gom._active_game_objects){
				_active_game_objects.insert(std::move(go));
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
			if (!_game_object_update_process) { //�Q�[���I�u�W�F�N�g�X�V�v���Z�X���擾�ł��ĂȂ������炷��B����ł��ł��Ȃ������玸�s
				_game_object_update_process = _scene->game_process_manager().GetSystemProcess<system_processes::GameObjectUpdateProcess>();
				if (!_game_object_update_process) { return false; }
			}
			auto it = _inactive_game_objects.find(id);
			if (it == _inactive_game_objects.end()) { return false; }
			it->second->_Activated();
			_active_game_objects.insert(*it);
			_game_object_update_process->Resist(id, it->second); //�Q�[���I�u�W�F�N�g�X�V�v���Z�X�ɓo�^
			_inactive_game_objects.erase(it);
			return true;
		}

		bool GameObjectManager::InActivate(int id)
		{			
			auto it = _active_game_objects.find(id);
			if (it == _active_game_objects.end()) { return false; }
			RemoveFromUpdateProcess_(id); //�A�b�v�f�[�g�v���Z�X����폜
			it->second->_InActivated();
			_inactive_game_objects.insert(*it);
			_active_game_objects.erase(it);
			return true;
		}

		bool GameObjectManager::Remove(int id)
		{
			auto it = _active_game_objects.find(id);
			if (it == _active_game_objects.end()) {
				it = _inactive_game_objects.find(id);
				if (it==_inactive_game_objects.end()) { return false; }
				else {
					it->second->_Finalize();
					_inactive_game_objects.erase(it);
					return true;
				}
			}
			else {
				RemoveFromUpdateProcess_(id); //�A�b�v�f�[�g�v���Z�X����폜
				it->second->_InActivated(); //�A�N�e�B�u���X�g�ɂ������Q�[���I�u�W�F�N�g�́A�������������s���Ă���j������
				it->second->_Finalize();
				_active_game_objects.erase(it);
				return true;
			}
		}

		void GameObjectManager::RemoveAllGameObjects()
		{
			for (auto& go : _active_game_objects) {
				go.second->_InActivated();
				go.second->_Finalize();
			}
			_active_game_objects.clear();
			for (auto& go : _inactive_game_objects) {
				go.second->_Finalize();
			}
			_inactive_game_objects.clear();
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
			if (!_game_object_update_process) { //�Q�[���I�u�W�F�N�g�X�V�v���Z�X���擾�ł��ĂȂ������炷��B����ł��ł��Ȃ������玸�s
				_game_object_update_process = _scene->game_process_manager().GetSystemProcess<system_processes::GameObjectUpdateProcess>();
				if (!_game_object_update_process) { return false; }
			}
			return _game_object_update_process->Remove(id); //�Q�[���I�u�W�F�N�g�X�V�v���Z�X����o�^����
		}

	}
}