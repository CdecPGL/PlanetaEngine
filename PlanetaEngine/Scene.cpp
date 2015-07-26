#include "Scene.h"
#include "GameObjectManager.h"
#include "ProcessManager.h"
#include "UIManager.h"

namespace PlanetaEngine{
	namespace Core{

		Scene::Scene() :_game_object_manager(std::make_unique<Game::GameObjectManager>()), _process_manager(std::make_unique<Game::ProcessManager>()), _ui_manager(std::make_unique<UI::UIManager>())
		{

		}

		Scene::~Scene()
		{

		}

		bool Scene::Init()
		{
			return false;
		}

		bool Scene::End()
		{
			return true;
		}

		void Scene::Update()
		{
			process_manager().Update(); //プロセス実行
			game_object_manager().Process(); //ゲームオブジェクトマネージャ更新
			ui_manager().Process(); //UIマネージャ更新
			process_manager().Process(); //プロセスマネージャ更新
		}

	}
}