#include "Scene.h"
//Modules
#include "GameObjectManager.h"
#include "TaskManager.h"
#include "CollisionWorld.h"

#include "SystemLog.h"
#include "ISceneManagerAccessor.h"
#include "IGameAccessor.h"
#include "SceneData.h"
#include "Camera.h"
#include "ScreenDrawer2D.h"
#include "ScreenDrawerGUI.h"

namespace planeta_engine{
	namespace core{

		Scene::Scene(IGameAccessor& engine) :game_(engine)
			,game_object_manager_(std::make_unique<GameObjectManager>()), game_process_manager_(std::make_unique<TaskManager>(game_)),collision_world_(std::make_unique<CollisionWorld>())
			,camera_(std::make_unique<Camera>())
		{
		}

		Scene::~Scene()
		{

		}

		bool Scene::Initialize()

		{
			//���W���[����������
			if (ForEachSceneModule_([](core::SceneModule& sm) {return sm.Initialize(); }) && camera_->Initialize()) {
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
			game_object_manager_->Finalize(); //�Q�[���I�u�W�F�N�g�ł͏I�����ɃQ�[���v���Z�X���Q�Ƃ�����̂�����̂�TaskManager����ɏI���������s���B
			game_process_manager_->Finalize();
			return true;
		}

		void Scene::Update()
		{
			try {
				game_process_manager_->ExcuteTask(); //�Q�[���v���Z�X���s
			}
			catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("TaskManager::Update�Ŗ�����WeakPointer���Q�Ƃ���܂����B") + e.what(), "Scene::Update");
				game_.scene_manager().ErrorOccured();
				return;
			}try {
				game_object_manager_->Update(); //�Q�[���I�u�W�F�N�g�}�l�[�W���X�V
			}
			catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("GameObjectManager::Update�Ŗ�����WeakPointer���Q�Ƃ���܂����B") + e.what(), "Scene::Update");
				game_.scene_manager().ErrorOccured();
				return;
			}
			game_process_manager_->Update(); //�v���Z�X�}�l�[�W���X�V
			//�J�����X�V
			camera_->Update();
		}

		bool Scene::ForEachSceneModule_(std::function<bool(SceneModule&)>&& proc) {
			return proc(*game_process_manager_)
				&& proc(*game_object_manager_);
		}

		void Scene::RegisterSceneDataToModules() {
			ForEachSceneModule_([&scene_data = scene_data_](core::SceneModule& sm) {sm.SetSceneData(*scene_data); return true; });
		}

		void Scene::PrepareSceneData() {
			scene_data_ = std::shared_ptr<SceneData>(new SceneData{ *game_object_manager_,*game_process_manager_,nullptr,std::make_shared<ScreenDrawer2D>(game_.screen()) });
		}

	}
}