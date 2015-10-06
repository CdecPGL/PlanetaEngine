#include "Game.h"

namespace planeta_engine {
	namespace core {
		Game::Game():_scene_manager(std::make_unique<core::SceneManager>(*this)), _key_input_manager(std::make_unique<core::KeyInputManager>())
		{

		}

		bool Game::Initialize()
		{
			_key_input_manager->Initialize();
			return _scene_manager->Initialize();
		}

		void Game::Finalize()
		{
			_scene_manager->Finalize();
		}

		Game::Status Game::Update()
		{
			_key_input_manager->Update();
			Status status;
			switch (_scene_manager->Process())
			{
			case SceneManager::SceneStatus::Continue:status = Status::Continue; break;
			case SceneManager::SceneStatus::Error:status = Status::Error; break;
			case SceneManager::SceneStatus::Quit:status = Status::Quit; break;
			default:return Status::Error;
			}
			return status;
		}

	}
}
