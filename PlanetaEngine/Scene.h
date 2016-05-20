#pragma once

#include <memory>
#include <string>
#include <functional>
#include "Object.h"
#include "SceneAccessorForSetUp.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

#include "Camera.h"

namespace planeta_engine{
	class GameObjectManager;
	class TaskManager;
	namespace core{
		class IGameAccessor;
		class Screen;
		struct SceneData;
		class CollisionWorld;
		class Scene : public Object,public std::enable_shared_from_this<Scene>
			,private utility::NonCopyable<Scene>{
		public:
			Scene(IGameAccessor& game);
			~Scene();
			/*�V�[���̏�����*/
			bool Initialize();
			/*�V�[���̏I������*/
			bool Finalize();
			/*�V�[���̍X�V*/
			void Update();

			/*�Q�[���N���X�ւ̃A�N�Z�X*/
			IGameAccessor& game_accessor() { return game_; }

			/*�ݒ�֐�*/
			SceneData& RefSceneData() { return *scene_data_; }

			/*�������p�֐�*/
			void PrepareSceneData();
			void RegisterSceneDataToModules();

		private:
			IGameAccessor& game_;
			std::unique_ptr<TaskManager> game_process_manager_; //�Q�[���v���Z�X�}�l�[�W��
			std::unique_ptr<GameObjectManager> game_object_manager_; //�Q�[���I�u�W�F�N�g�}�l�[�W��
			std::unique_ptr<CollisionWorld> collision_world_; //�R���W�������[���h
			bool ForEachSceneModule_(std::function<bool(SceneModule&)>&& proc); //�V�[�����W���[���ɑ����K�p����

			std::unique_ptr<Camera> camera_;

			std::shared_ptr<SceneData> scene_data_; //�V�[���f�[�^
		};
	}
}