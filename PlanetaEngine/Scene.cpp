#include "Scene.h"
//Modules
#include "GameObjectManager.h"
#include "TaskManager.h"
#include "CollisionWorld.h"
#include "GameObjectDrawSystem.h"
#include "AnimationSystem.h"
#include "TransformSystem.h"

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
			,game_object_manager_(std::make_unique<GameObjectManager>()), task_manager_(std::make_unique<TaskManager>(game_)),collision_world_(std::make_unique<CollisionWorld>()),gameobject_draw_system_(std::make_unique<GameObjectDrawSystem>()),animation_system_(std::make_unique<AnimationSystem>()),transform_system_(std::make_unique<TransformSystem>())
		{
			scene_module_list_ = {
				game_object_manager_.get(),
				collision_world_.get(),
				gameobject_draw_system_.get(),
				animation_system_.get(),
				transform_system_.get(),
				task_manager_.get(),
			};
		}

		Scene::~Scene()
		{

		}

		bool Scene::Initialize()

		{
			//モジュールを初期化
			if (IterateSceneModule_([](core::SceneModule& sm) {return sm.Initialize(); })) {
				return true;
			}
			else {
				debug::SystemLog::instance().LogError("Sceneの初期化に失敗しました。", "Scene::Initialize");
				return false;
			}
		}

		bool Scene::Finalize()
		{
			//モジュールの終了処理を行う
			ReverseIterateSceneModule_([](SceneModule& sm) {sm.Finalize(); return true; });
			return true;
		}

		void Scene::Update()
		{
			try {
				task_manager_->ExcuteTask(); //タスク実行
			}
			catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("TaskManager::Updateで無効なWeakPointerが参照されました。") + e.what(), __FUNCTION__);
				game_.scene_manager().ErrorOccured();
				return;
			}try {
				//各シーンモジュールの更新
				IterateSceneModule_([](SceneModule& sm) {sm.Update(); return true; });
			}
			catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("シーンモジュールの更新において無効なWeakPointerが参照されました。") + e.what(), __FUNCTION__);
				game_.scene_manager().ErrorOccured();
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

		void Scene::PrepareSceneData() {
			scene_data_ = std::shared_ptr<SceneData>(new SceneData{ *game_object_manager_,*task_manager_,*collision_world_,*gameobject_draw_system_ ,*animation_system_,*transform_system_ });
		}

	}
}