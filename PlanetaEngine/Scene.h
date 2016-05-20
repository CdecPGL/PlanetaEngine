#pragma once

#include <memory>
#include <string>
#include <functional>
#include "Object.h"
#include "SceneAccessorForSetUp.h"
#include "ScenePublicInterface.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

#include "GameObjectManager.h"
#include "TaskManager.h"
#include "Camera.h"

namespace planeta_engine{
	namespace core{
		class IGameAccessor;
		class Screen;
		struct SceneData;
		class Scene : public Object,public std::enable_shared_from_this<Scene>
			,public ScenePublicInterface,private utility::NonCopyable<Scene>{
		public:
			Scene(IGameAccessor& game);
			~Scene();
			/*�V�[���̏�����*/
			bool Initialize();
			/*�V�[���̏I������*/
			bool Finalize();
			/*�V�[���̍X�V*/
			void Update();
			/*�v���Z�X�}�l�[�W���̃C���X�^���X���擾*/
			TaskManager& game_process_manager()override { assert(game_process_manager_ != nullptr); return *game_process_manager_; }
			/*�Q�[���I�u�W�F�N�g�}�l�[�W���̃C���X�^���X���擾*/
			GameObjectManager& game_object_manager()override { assert(game_object_manager_ != nullptr); return *game_object_manager_; }

			Camera& camera()override { return *camera_; }
			/*�Q�[���N���X�ւ̃A�N�Z�X*/
			IGameAccessor& game_accessor() { return game_; }

			/*�ݒ�֐�*/
			SceneData& RefSceneData() { return *scene_data_; }

			/*�������p�֐�*/
			void RegisterSceneInterfaceToModules();
			void PrepareSceneData();
			void RegisterSceneDataToModules();
		private:
			IGameAccessor& game_;
			/*�V�[�����W���[��*/
			std::unique_ptr<TaskManager> game_process_manager_; //�Q�[���v���Z�X�}�l�[�W��
			std::unique_ptr<GameObjectManager> game_object_manager_; //�Q�[���I�u�W�F�N�g�}�l�[�W��
			bool ForEachSceneModule_(std::function<bool(SceneModule&)>&& proc); //�V�[�����W���[���ɑ����K�p����

			std::unique_ptr<Camera> camera_;

			std::unique_ptr<SceneData> scene_data_; //�V�[���f�[�^
		};
	}
}