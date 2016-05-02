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
		}

		Scene::~Scene()
		{

		}

		bool Scene::Initialize()

		{
			//モジュールを初期化
			if (ForEachSceneModule_([](core::SceneModule& sm) {return sm.Initialize(); })) {
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
			game_object_manager_->Finalize(); //ゲームオブジェクトでは終了時にゲームプロセスを参照するものがあるのでTaskManagerより先に終了処理を行う。
			task_manager_->Finalize();
			return true;
		}

		void Scene::Update()
		{
			try {
				task_manager_->ExcuteTask(); //ゲームプロセス実行
			}
			catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("TaskManager::Updateで無効なWeakPointerが参照されました。") + e.what(), "Scene::Update");
				game_.scene_manager().ErrorOccured();
				return;
			}try {
				game_object_manager_->Update(); //ゲームオブジェクトマネージャ更新
			}
			catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("GameObjectManager::Updateで無効なWeakPointerが参照されました。") + e.what(), "Scene::Update");
				game_.scene_manager().ErrorOccured();
				return;
			}
			task_manager_->Update(); //プロセスマネージャ更新
		}

		bool Scene::ForEachSceneModule_(std::function<bool(SceneModule&)>&& proc) {
			return proc(*task_manager_)
				&& proc(*game_object_manager_);
		}

		void Scene::RegisterSceneDataToModules() {
			ForEachSceneModule_([&scene_data = scene_data_](core::SceneModule& sm) {sm.SetSceneData(scene_data); return true; });
		}

		void Scene::PrepareSceneData() {
			scene_data_ = std::shared_ptr<SceneData>(new SceneData{ *game_object_manager_,*task_manager_,*collision_world_,*gameobject_draw_system_ ,*animation_system_,*transform_system_ });
		}

	}
}