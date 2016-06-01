#include "Scene.h"
//Modules
#include "GameObjectManager.h"
#include "TaskManager.h"
#include "CollisionWorld.h"
#include "GameObjectDrawSystem.h"
#include "TransformSystem.h"

#include "SystemLog.h"
#include "SceneData.h"
#include "ScreenDrawer2D.h"
#include "ScreenDrawerGUI.h"

namespace planeta{
	namespace core{

		Scene::Scene() :game_object_manager_(std::make_unique<GameObjectManager>()), task_manager_(std::make_unique<TaskManager>()),collision_world_(std::make_unique<CollisionWorld>()),gameobject_draw_system_(std::make_unique<GameObjectDrawSystem>()),transform_system_(std::make_unique<TransformSystem>())
		{
			scene_module_list_ = {
				game_object_manager_.get(),
				collision_world_.get(),
				gameobject_draw_system_.get(),
				transform_system_.get(),
				task_manager_.get(),
			};
		}

		Scene::~Scene()
		{

		}

		bool Scene::Initialize()

		{
			//���W���[����������
			if (IterateSceneModule_([](core::SceneModule& sm) {return sm.Initialize(); })) {
				return true;
			}
			else {
				debug::SystemLog::instance().LogError("Scene�̏������Ɏ��s���܂����B", "Scene::Initialize");
				return false;
			}
		}

		bool Scene::Finalize()
		{
			//���W���[���̏I���������s��
			ReverseIterateSceneModule_([](SceneModule& sm) {sm.Finalize(); return true; });
			return true;
		}

		void Scene::Update()
		{
			try {
				task_manager_->ExcuteTask(); //�^�X�N���s
			}
			catch (util::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("TaskManager::Update�Ŗ�����WeakPointer���Q�Ƃ���܂����B") + e.what(), __FUNCTION__);
				return;
			}try {
				//�e�V�[�����W���[���̍X�V
				IterateSceneModule_([](SceneModule& sm) {sm.Update(); return true; });
			}
			catch (util::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("�V�[�����W���[���̍X�V�ɂ����Ė�����WeakPointer���Q�Ƃ���܂����B") + e.what(), __FUNCTION__);
				return;
			}
		}

		bool Scene::IterateSceneModule_(std::function<bool(SceneModule&)>&& proc) {
			for (auto it = scene_module_list_.begin(); it != scene_module_list_.end(); ++it) {
				if (proc(**it) == false) { return false; }
			}
			return true;
		}

		bool Scene::ReverseIterateSceneModule_(std::function<bool(SceneModule&)>&& proc) {
			for (auto it = scene_module_list_.rbegin(); it != scene_module_list_.rend(); ++it) {
				if (proc(**it) == false) { return false; }
			}
			return true;
		}

		void Scene::RegisterSceneDataToModules() {
			IterateSceneModule_([&scene_data = scene_data_](core::SceneModule& sm) {sm.SetSceneData(scene_data); return true; });
		}

		void Scene::PrepareSceneData(const util::NonOwingPointer<ISceneManagerAccessor> scn_mgr) {
			scene_data_ = std::shared_ptr<SceneData>(new SceneData{ *game_object_manager_,*task_manager_,*collision_world_,*gameobject_draw_system_ ,*transform_system_, *scn_mgr });
		}

		void Scene::SetCollisionGroupMatrix(const std::shared_ptr<CollisionGroupMatrix>& col_g_marix) {
			collision_group_matrix_ = col_g_marix;
		}

		std::shared_ptr<CollisionGroupMatrix> Scene::GetCollisionGroupMatrix() {
			return collision_group_matrix_;
		}

	}
}