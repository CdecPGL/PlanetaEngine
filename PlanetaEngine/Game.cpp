#include "Game.h"
#include "SoundManager.h"
#include "CollisionGroupMatrix.h"
#include "DrawManager.h"
#include "GameProcessKeyPositionDefinition.h"

namespace planeta_engine {
	namespace core {
		Game::Game():_scene_manager(std::make_unique<core::SceneManager>(*this)), _key_input_manager(std::make_unique<core::KeyInputManager>())
			,collision_group_matrix_(std::make_shared<core::CollisionGroupMatrix>())
			,game_process_key_position_definition_(std::make_shared<GameProcessKeyPositionDefinition>())
		{

		}

		Game::~Game() = default;

		bool Game::Initialize()
		{
			return InitializeSystem() && InitializeGame();
		}

		void Game::Finalize()
		{
			FinalizeGame();
			FinalizeSystem();
		}

		Game::Status Game::Update()
		{
			core::SoundManager::instance().Update();
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

		bool Game::InitializeSystem() {
			screen_ = DrawManager::instance().CreateScreen(); //スクリーンを作成
			_key_input_manager->Initialize();
			return _scene_manager->Initialize();
		}

		void Game::FinalizeSystem() {
			_scene_manager->Finalize();
			DrawManager::instance().DisposeScreen(screen_); //スクリーンを破棄
		}

		const std::list<std::string>& Game::GetGameProcessKeyPositionList() const {
			return game_process_key_position_definition_->GetKeyPositionIDList();
		}

	}
}
