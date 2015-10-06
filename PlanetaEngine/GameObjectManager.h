#pragma once

#include <unordered_map>
#include <memory>
#include "Object.h"
#include "IGameObjectManagerAccessor.h"
#include "IGameObjectManagerSetup.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace system_processes {
		class GameObjectUpdateProcess;
	}
	namespace core {
		class ISceneAccessForGameObject;
	}
	namespace game{
		class GameObject;
		class GameObjectSetUpper;
		class GameObjectManager final: public core::Object,public IGameObjectManagerAccessor,public IGameObjectManagerSetup{
		public:
			GameObjectManager() :_id_counter(0){};
			~GameObjectManager()=default;
			//���[�U�A�N�Z�X�\�֐�
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper)override;
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper,const std::string& name)override;
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper)override;
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name)override;

			//�V�X�e���֐�
			/*�e��}�l�[�W���|�C���^�[���Z�b�g*/
			void SetManagerPointer(const utility::WeakPointer<core::ISceneAccessForGameObject>& scene) { _scene = scene; }
			/*������*/
			bool Initialize();
			/*�I������*/
			bool Finalize();
			/*�Ǘ�����*/
			bool Process();
			/*�Q�[���I�u�W�F�N�g�o�^(���������s���AID��Ԃ�)*/
			int Resist(const std::shared_ptr<GameObject>& go);
			int Resist(const std::shared_ptr<GameObject>& go,const std::string& name);
			/*�Q�[���I�u�W�F�N�g�o�^����(�I���������s��)*/
			bool Remove(int id);
			/*�L����*/
			bool Activate(int id);
			/*������*/
			bool InActivate(int id);
			void TakeOver(const GameObjectManager& gom); //�Q�[���I�u�W�F�N�g�̈��p������(������)
			/*���ׂẴQ�[���I�u�W�F�N�g��j��*/
			void RemoveAllGameObjects();

		private:
			std::unordered_map<int, std::shared_ptr<GameObject>> _active_game_objects;
			std::unordered_map<int, std::shared_ptr<GameObject>> _inactive_game_objects;
			std::unordered_map<std::string, int> _name_id_map;
			int _id_counter;
			utility::WeakPointer<core::ISceneAccessForGameObject> _scene;
			utility::WeakPointer<system_processes::GameObjectUpdateProcess> _game_object_update_process;
			bool RemoveFromUpdateProcess_(int id);
		};
	}
}