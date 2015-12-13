#pragma once

#include <unordered_map>
#include <memory>
#include "SceneModule.h"
#include "GameObjectManagerPublicInterface.h"
#include "WeakPointer.h"

namespace planeta_engine{
	namespace core {
		class ScenePublicInterface;
		struct SceneData;
		class SceneAccessorForGameObject;
		struct SceneDataForGameObject;
	}
	namespace game{
		class GameObject;
		class GameObjectSetUpper;
		class GameObjectManager final: public core::SceneModule
			,public GameObjectManagerPublicInterface{
		public:
			GameObjectManager();
			~GameObjectManager();
			//���[�U�A�N�Z�X�\�֐�
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper)override;
			utility::WeakPointer<IGameObjectAccessor> CreateGameObject(GameObjectSetUpper& game_object_setupper,const std::string& name)override;
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper)override;
			utility::WeakPointer<IGameObjectAccessor> CreateAndActivateGameObject(GameObjectSetUpper& game_object_setupper, const std::string& name)override;

			//�V�X�e���֐�
			/*������*/
			bool Initialize()override;
			/*�I������*/
			void Finalize()override;
			/*�V�[���C���^�[�t�F�C�X�Z�b�g*/
			void SetSceneInterface(core::ScenePublicInterface& spi)override;
			/*�V�[���f�[�^�Z�b�g*/
			void SetSceneData(const core::SceneData& scene_data)override;

			/*�Ǘ�����*/
			bool Process();
			/*�X�V*/
			void Update();
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

			/*�V�[���f�[�^�Q��*/
			core::SceneDataForGameObject& RefSceneData() { return *scene_data_; }
		private:
			GameObjectManager(const GameObjectManager&) = delete;
			GameObjectManager(GameObjectManager&&) = delete;
			GameObjectManager& operator=(const GameObjectManager&) = delete;
			GameObjectManager& operator=(GameObjectManager&&) = delete;

			std::unordered_map<int, std::shared_ptr<GameObject>> active_game_objects_;
			std::unordered_map<int, std::shared_ptr<GameObject>> inactive_game_objects_;
			std::unordered_map<std::string, int> name_id_map_;
			std::vector<std::shared_ptr<GameObject>> garbage_;
			void RemoveProc_();
			int _id_counter;
			std::shared_ptr<core::SceneAccessorForGameObject> scene_accessor_;
			std::unique_ptr<core::SceneDataForGameObject> scene_data_;
		};
	}
}