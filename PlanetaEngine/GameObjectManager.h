#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include "SceneModule.h"
#include "GameObjectManagerPublicInterface.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

namespace planeta_engine{
	class SceneAccessorForGameObject;
	class GameObjectBase;
	class IGameObject;
	namespace core {
		class ScenePublicInterface;
		struct SceneData;
		struct SceneDataForGameObject;
	}
	namespace game{
		class GameObjectSetUpper;
		class GameObjectManager final: public core::SceneModule
			,public GameObjectManagerPublicInterface,private utility::NonCopyable<GameObjectManager>{
		public:
			GameObjectManager();
			~GameObjectManager();
			//���[�U�A�N�Z�X�\�֐�
			utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id)override;
			utility::WeakPointer<IGameObject> CreateGameObject(const std::string& game_object_create_id,const std::string& name)override;

			//�ݒ�p�֐�
			void RegisterGameObjectCreator(const std::string& game_object_create_id, std::function<std::shared_ptr<GameObjectBase>()>&& creator)override;

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
			/*���x�K�p*/
			void ApplyVelocityToGameObject();
			/*�Q�[���I�u�W�F�N�g�o�^(���������s���AID��Ԃ�)*/
			int RegisterGameObject(const std::shared_ptr<GameObjectBase>& go);
			int RegisterGameObject(const std::shared_ptr<GameObjectBase>& go,const std::string& name);
			/*�Q�[���I�u�W�F�N�g�o�^����(�I���������s��)*/
			bool RemoveGameObject(int id);
			/*�L����*/
			bool ActivateGameObject(int id);
			/*������*/
			bool InActivateGameObject(int id);
			void TakeOver(const GameObjectManager& gom); //�Q�[���I�u�W�F�N�g�̈��p������(������)
			/*���ׂẴQ�[���I�u�W�F�N�g��j��*/
			void RemoveAllGameObjects();

			/*�V�[���f�[�^�Q��*/
			core::SceneDataForGameObject& RefSceneData() { return *scene_data_; }
		private:
			std::unordered_map<int, std::shared_ptr<GameObjectBase>> active_game_objects_;
			std::unordered_map<int, std::shared_ptr<GameObjectBase>> inactive_game_objects_;
			std::unordered_map<std::string, int> name_id_map_;
			std::vector<std::shared_ptr<GameObjectBase>> garbage_;
			std::unordered_map<std::string, std::function<bool(GameObjectBase&)>> setupper_map_; //�Q�[���I�u�W�F�N�g�Z�b�g�A�b�p�[�}�b�v
			bool SetUpGameObject_(GameObjectBase& game_object,const std::string& setup_id)const;
			void RemoveProc_();
			int _id_counter;
			std::shared_ptr<SceneAccessorForGameObject> scene_accessor_;
			std::unique_ptr<core::SceneDataForGameObject> scene_data_;
		};
	}
}