#include "PlanetaEngine.h"
#include "InitEnd.h"
#include "SceneManager.h"
#include "SystemCounter.h"
#include "ResourceManager.h"
#include "SystemLog.h"
#include "DxLib.h"
#include "DrawManager.h"
#include "NullWeakPointerException.h"
#include "KeyInputManager.h"
#include "Game.h"

//#include "GlobalNewDeleteDefinition.h"

namespace planeta_engine {
	namespace core {
		PlanetaEngine::PlanetaEngine() :_is_initialized(false),_game(std::make_unique<Game>())
		{

		}

		PlanetaEngine::~PlanetaEngine()
		{
			if (_is_initialized) { Finalize(); }
		}

		bool PlanetaEngine::Initialize()
		{
			using namespace planeta_engine::core::init_end;

//			memory_allocator.ReserveMemory(1024 * 1024 * 10);

			CreateSystemDirectory();
			if (!InitializeDebugSystem()) { //デバッグシステムの初期化
				return false;
			}
			if (InstantiateSingletonManagers() //シングルトンマネージャのインスタンス化
				&&InitDxLibrary() //DXライブラリの初期化
				&& SetUpSingletonManagers() //シングルトンマネージャのセットアップ
				&& _InitializeGame() //固有初期化
				&& InitializeSingletonManagers() //シングルトンマネージャの初期化
				&& _game->Initialize() //ゲームクラスの初期化
				) {
				debug::SystemLog::instance().LogMessage("PlanetaEngineの初期化に成功しました。", "PlanetaEngine::Initialize");
				_is_initialized = true;
			}
			else {
				debug::SystemLog::instance().LogError("PlanetaEngineの初期化に失敗しました。", "PlanetaEngine::Initialize");
				_is_initialized = false;
				return false;
			}
			//スタートアップシーンの開始
			if (_game->scene_manager().LoadAndTransitionScene(_StartUpSceneName())) {
				debug::SystemLog::instance().LogMessage(std::string("スタートアップシーン(") + _StartUpSceneName() + ")を開始します。", "PlanetaEngine::Initialize");
				return true;
			}
			else {
				debug::SystemLog::instance().LogError(std::string("スタートアップシーン(")+_StartUpSceneName()+")の開始に失敗しました。", "PlanetaEngine::Initialize");
				return false;
			}
		}

		void PlanetaEngine::Finalize()
		{
			using namespace planeta_engine::core::init_end;

			_game->Finalize(); //ゲームクラスの終了処理
			FinalizeSingletonManagers(); //シングルトンマネージャの終了処理
			DisposeSingletonManagers(); //シングルトンマネージャの破棄
			EndDxLibrary(); //DXライブラリの終了処理
			debug::SystemLog::instance().LogMessage("PlanetaEngineの終了化処理を行いました。", "PlanetaEngine::Finalize");
			FinalizeDebugStstem(); //デバッグシステムの終了処理
			_is_initialized = false;
		}

		PlanetaEngine::Status PlanetaEngine::Update()
		{
			if (ProcessMessage() < 0) { return Status::Quit; } //DXライブラリの更新処理
			
			Status status;

			Game::Status gstatue = _game->Update();

			switch (gstatue)
			{
			case planeta_engine::core::Game::Status::Continue:status = Status::Continue; break;
			case planeta_engine::core::Game::Status::Quit:status = Status::Quit; break;
			case planeta_engine::core::Game::Status::Error:status = Status::Error; break;
			default:status = Status::Error; break;
			}

			debug::SystemCounter::instance().IncrementFrameCount();
			if (status == Status::Continue) {
				if (DrawManager::instance().Update()==false) { status = Status::Error; }
			}
			return status;
		}

		IGameSetUp& PlanetaEngine::game_setup_access()
		{
			return *_game;
		}

	}
}