#include <cassert>

#include "DxLib.h"

#include "PlanetaEngine.h"
#include "InitEnd.h"
#include "SystemLog.h"
#include "SystemCounter.h"
#include "Game.h"
#include "DrawManager.h"

namespace planeta_engine {
	using namespace core;
	PlanetaEngine::PlanetaEngine() :is_initialized_(false) {

	}

	PlanetaEngine::~PlanetaEngine() {
		assert(!is_initialized_);
	}

	bool PlanetaEngine::Initialize() {
		if (is_initialized_) { assert(false); return false; }
		assert(game_ != nullptr);
		init_end::CreateSystemDirectory(); //システムディレクトリ作成
		if (InitializeDebugSystem() //デバッグシステム初期化
			&& init_end::LoadConfigData() //設定データ読み込み
			&& InitializeEngine() //エンジン初期化
			&& game_->Initialize()) //ゲーム初期化
		{
			debug::SystemLog::instance().LogMessage("PlanetaEngineを初期化しました。", __FUNCTION__);
			is_initialized_ = true;
			return true;
		} else {
			debug::SystemLog::instance().LogError("PlanetaEngineの初期化に失敗しました。", __FUNCTION__);
			is_initialized_ = true;
			Finalize();
			return false;
		}
	}

	bool PlanetaEngine::Finalize() {
		if (!is_initialized_) { assert(false); return false; }
		assert(game_ != nullptr);
		game_->Finalize();
		FinalzieEngine();
		debug::SystemLog::instance().LogMessage("PlanetaEngineを終了しました。", __FUNCTION__);
		FinalizeDebugSystem();
		is_initialized_ = false;
		return true;
	}

	PlanetaEngine::Status PlanetaEngine::Update() {
		if (ProcessMessage() < 0) { return Status::Quit; } //DXライブラリの更新処理

		Status status;

		core::Game::Status gstatue = game_->Update();

		switch (gstatue) {
		case planeta_engine::core::Game::Status::Continue:status = Status::Continue; break;
		case planeta_engine::core::Game::Status::Quit:status = Status::Quit; break;
		case planeta_engine::core::Game::Status::Error:status = Status::Error; break;
		default:status = Status::Error; break;
		}

		debug::SystemCounter::instance().IncrementFrameCount();
		if (status == Status::Continue) {
			if (DrawManager::instance().Update() == false) { status = Status::Error; }
		}
		return status;
	}

	bool PlanetaEngine::InitializeEngine() {
		using namespace planeta_engine::core::init_end;

		if (InitDxLibrary() //DXライブラリの初期化
			&& SetUpSingletonManagers() //シングルトンマネージャのセットアップ
			&& InitializeSingletonManagers() //シングルトンマネージャの初期化
			) {
			debug::SystemLog::instance().LogMessage("PlanetaEngine本体を初期化しました。", __FUNCTION__);
			return true;
		} else {
			debug::SystemLog::instance().LogError("PlanetaEngine本体の初期化に失敗しました。", __FUNCTION__);
			return false;
		}
	}

	void PlanetaEngine::FinalzieEngine() {
		using namespace planeta_engine::core::init_end;
		FinalizeSingletonManagers(); //シングルトンマネージャの終了処理
		EndDxLibrary(); //DXライブラリの終了処理
		debug::SystemLog::instance().LogMessage("PlanetaEngine本体を終了しました。", __FUNCTION__);
	}

	bool PlanetaEngine::InitializeDebugSystem() {
		return init_end::InitializeDebugSystem();
	}

	void PlanetaEngine::FinalizeDebugSystem() {
		init_end::FinalizeDebugStstem();
	}
}