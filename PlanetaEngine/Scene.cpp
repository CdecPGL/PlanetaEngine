#include "Scene.h"
#include "GameObjectManager.h"
#include "GameProcessManager.h"
#include "UIManager.h"
#include "SystemLog.h"
#include "ISceneManagerAccessor.h"
#include "IGameAccessor.h"

namespace planeta_engine{
	namespace core{

		Scene::Scene(IGameAccessor& engine) :game_(engine),game_object_manager_(std::make_unique<game::GameObjectManager>()), game_process_manager_(std::make_unique<game::GameProcessManager>(game_)), ui_manager_(std::make_unique<game::UIManager>())
		{
			game_object_manager_->SetScene(*this);
			game_process_manager_->SetScene(*this);
			ui_manager_->SetScene(*this);
		}

		Scene::~Scene()
		{

		}

		bool Scene::Initialize()
		{
			if (game_object_manager_->Initialize()) {
				return true;
			}
			else {
				debug::SystemLog::instance().LogError("Sceneの初期化に失敗しました。", "Scene::Initialize");
				return false;
			}
		}

		bool Scene::Finalize()
		{
			game_object_manager_->Finalize();
			ui_manager_->Finalize();
			game_process_manager_->Finalize();
			return true;
		}

		void Scene::Update()
		{
			try {
				game_process_manager_->Update(); //プロセス実行
			}
			catch (utility::NullWeakPointerException& e) {
				debug::SystemLog::instance().LogError(std::string("GameProcessManager::Updateで無効なWeakPointerが参照されました。") + e.what(), "Scene::Update");
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
		}

	}
}