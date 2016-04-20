#include "Scene.h"
#include "GameObjectManager.h"
#include "TaskManager.h"
#include "GUIManager.h"
#include "SystemLog.h"
#include "ISceneManagerAccessor.h"
#include "IGameAccessor.h"
#include "SceneData.h"
#include "Camera.h"
#include "ScreenDrawer2D.h"
#include "ScreenDrawerGUI.h"

namespace planeta_engine{
	namespace core{

		Scene::Scene(IGameAccessor& engine) :game_(engine),game_object_manager_(std::make_unique<game::GameObjectManager>()), game_process_manager_(std::make_unique<game::TaskManager>(game_)), ui_manager_(std::make_unique<game::GUIManager>()),camera_(std::make_unique<Camera>()),scene_data_(std::make_unique<SceneData>())
		{
		}

		Scene::~Scene()
		{

		}

		bool Scene::Initialize()

		{
			//モジュールを初期化
			if (ForEachSceneModule_([](core::SceneModule& sm) {return sm.Initialize(); }) && camera_->Initialize()) {
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
			ui_manager_->Finalize();
			game_process_manager_->Finalize();
			return true;
		}

		void Scene::Update()
		{
			try {
				game_process_manager_->Update(); //ゲームプロセス実行
			}
			catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("TaskManager::Updateで無効なWeakPointerが参照されました。") + e.what(), "Scene::Update");
				game_.scene_manager().ErrorOccured();
				return;
			}try {
				game_object_manager_->Process(); //ゲームオブジェクトマネージャ更新
			}
			catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("GameObjectManager::Updateで無効なWeakPointerが参照されました。") + e.what(), "Scene::Update");
				game_.scene_manager().ErrorOccured();
				return;
			}try {
				ui_manager_->Process(); //UIマネージャ更新
			}
			catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("UIManager::Updateで無効なWeakPointerが参照されました。") + e.what(), "Scene::Update");
				game_.scene_manager().ErrorOccured();
				return;
			}
			game_process_manager_->Process(); //プロセスマネージャ更新
			//カメラ更新
			camera_->Update();
		}

		bool Scene::ForEachSceneModule_(std::function<bool(SceneModule&)>&& proc) {
			return proc(*game_process_manager_)
				&& proc(*game_object_manager_)
				&& proc(*ui_manager_);
		}

		void Scene::RegisterSceneInterfaceToModules() {
			//シーンを各モジュールに登録
			ForEachSceneModule_([this](core::SceneModule& sm) {sm.SetSceneInterface(*this); return true; });
		}

		void Scene::RegisterSceneDataToModules() {
			ForEachSceneModule_([&scene_data = scene_data_](core::SceneModule& sm) {sm.SetSceneData(*scene_data); return true; });
		}

		void Scene::PrepareSceneData() {
			scene_data_->screen_drawer_2d = std::make_shared<ScreenDrawer2D>(game_.screen());
			scene_data_->screen_drawer_ui = std::make_shared<ScreenDrawerGUI>(game_.screen());
		}

	}
}