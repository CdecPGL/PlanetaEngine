#include "Game.h"
#include "SoundManager.h"
#include "CollisionGroupMatrix.h"
#include "RenderManager.h"

namespace planeta_engine {
	namespace core {
		Game::Game():_scene_manager(std::make_unique<core::SceneManager>(*this))
			,collision_group_matrix_(std::make_shared<core::CollisionGroupMatrix>())
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
			screen_ = RenderManager::instance().CreateScreen(); //�X�N���[�����쐬
			return _scene_manager->Initialize();
		}

		void Game::FinalizeSystem() {
			_scene_manager->Finalize();
			RenderManager::instance().DisposeScreen(screen_); //�X�N���[����j��
		}

		KeyInputManager& Game::key_input_manager() {
			return KeyInputManager::instance();
		}

	}
}
