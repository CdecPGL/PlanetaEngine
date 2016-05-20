#pragma once

#include <memory>
#include <string>
#include <functional>
#include <list>
#include "Object.h"
#include "WeakPointer.h"
#include "NonCopyable.h"

namespace planeta_engine{
	class GameObjectManager;
	class TaskManager;
	namespace core{
		class SceneSystemSetUpper;
		class IGameAccessor;
		struct SceneData;
		class CollisionWorld;
		class GameObjectDrawSystem;
		class TransformSystem;
		class SceneModule;
		class Scene : public Object,public std::enable_shared_from_this<Scene>
			,private utility::NonCopyable<Scene>{
			friend SceneSystemSetUpper;
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
			std::unique_ptr<TaskManager> task_manager_; //�Q�[���v���Z�X�}�l�[�W��
			std::unique_ptr<GameObjectManager> game_object_manager_; //�Q�[���I�u�W�F�N�g�}�l�[�W��
			std::unique_ptr<CollisionWorld> collision_world_; //�R���W�������[���h
			std::unique_ptr<GameObjectDrawSystem> gameobject_draw_system_; //�Q�[���I�u�W�F�N�g�`��V�X�e��
			std::unique_ptr<TransformSystem> transform_system_; //�g�����X�t�H�[���V�X�e��
			bool IterateSceneModule_(std::function<bool(SceneModule&)>&& proc); //�V�[�����W���[���ɑ����K�p����
			bool ReverseIterateSceneModule_(std::function<bool(SceneModule&)>&& proc); //�V�[�����W���[���ɑ����K�p����
			std::list<SceneModule*> scene_module_list_;

			std::shared_ptr<SceneData> scene_data_; //�V�[���f�[�^
		};
	}
}