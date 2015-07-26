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
			process_manager().Update(); //�v���Z�X���s
			game_object_manager().Process(); //�Q�[���I�u�W�F�N�g�}�l�[�W���X�V
			ui_manager().Process(); //UI�}�l�[�W���X�V
			process_manager().Process(); //�v���Z�X�}�l�[�W���X�V
		}

	}
}