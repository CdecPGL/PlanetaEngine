#pragma once

#include <unordered_map>
#include <memory>
#include "Object.h"
#include "GameObjectManagerPublicInterface.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace system_processes {
		class GameObjectUpdateProcess;
	}
	namespace core {
		class ScenePublicInterface;
		class SceneAccessorForGameObject;
	}
	namespace game{
		class GameObject;
		class GameObjectSetUpper;
		class GameObjectManager final: public core::Object,public GameObjectManagerPublicInterface{
		public:
			GameObjectManager();
			~GameObjectManager()=default;
			//���[�U�A�N�Z�X�\�֐�
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper)override;
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper,const std::string& name)override;
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper)override;
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name)override;

			//�V�X�e���֐�
			/*�V�[���Z�b�g*/
			void SetScene(core::ScenePublicInterface& spi);
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
			std::unordered_map<int, std::shared_ptr<GameObject>> active_game_objects_;
			std::unordered_map<int, std::shared_ptr<GameObject>> inactive_game_objects_;
			std::unordered_map<std::string, int> name_id_map_;
			int _id_counter;
			std::shared_ptr<core::SceneAccessorForGameObject> scene_accessor_;
			utility::WeakPointer<system_processes::GameObjectUpdateProcess> game_object_update_process_;
			bool RemoveFromUpdateProcess_(int id);
		};
	}
}