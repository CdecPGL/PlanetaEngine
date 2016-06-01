#pragma once

#include <memory>
#include <string>
#include <functional>
#include <list>
#include "Object.h"
#include "WeakPointer.h"
#include "NonCopyable.h"
#include "NonOwingPointer.h"

namespace planeta{
	class GameObjectManager;
	class TaskManager;
	class ISceneManagerAccessor;
	class CollisionGroupMatrix;
	namespace core{
		class SceneSystemSetUpper;
		struct SceneData;
		class CollisionWorld;
		class GameObjectDrawSystem;
		class TransformSystem;
		class SceneModule;
		class Scene : public Object,public std::enable_shared_from_this<Scene>
			,private util::NonCopyable<Scene>{
			friend SceneSystemSetUpper;
		public:
			Scene();
			~Scene();
			/*�V�[���̏�����*/
			bool Initialize();
			/*�V�[���̏I������*/
			bool Finalize();
			/*�V�[���̍X�V*/
			void Update();

			/*�ݒ�֐�*/
			SceneData& RefSceneData() { return *scene_data_; }
			std::shared_ptr<CollisionGroupMatrix> GetCollisionGroupMatrix();

			/*�������p�֐�*/
			void SetCollisionGroupMatrix(const std::shared_ptr<CollisionGroupMatrix>& col_g_marix);
			void PrepareSceneData(const util::NonOwingPointer<ISceneManagerAccessor> scn_mgr);
			void RegisterSceneDataToModules();

		private:
			std::unique_ptr<TaskManager> task_manager_; //�Q�[���v���Z�X�}�l�[�W��
			std::unique_ptr<GameObjectManager> game_object_manager_; //�Q�[���I�u�W�F�N�g�}�l�[�W��
			std::unique_ptr<CollisionWorld> collision_world_; //�R���W�������[���h
			std::unique_ptr<GameObjectDrawSystem> gameobject_draw_system_; //�Q�[���I�u�W�F�N�g�`��V�X�e��
			std::unique_ptr<TransformSystem> transform_system_; //�g�����X�t�H�[���V�X�e��
			bool IterateSceneModule_(std::function<bool(SceneModule&)>&& proc); //�V�[�����W���[���ɑ����K�p����
			bool ReverseIterateSceneModule_(std::function<bool(SceneModule&)>&& proc); //�V�[�����W���[���ɑ����K�p����
			std::list<SceneModule*> scene_module_list_;

			std::shared_ptr<SceneData> scene_data_; //�V�[���f�[�^
			std::shared_ptr<CollisionGroupMatrix> collision_group_matrix_;
		};
	}
}