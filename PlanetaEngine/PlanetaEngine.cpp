#include <cassert>
#include <functional>
#include <list>

#include "DxLib.h"

#include "InitFunctions.h"
#include "ProgramDefinitionData.h"

#include "PlanetaEngine.h"

#include "SystemLog.h"
#include "SystemTimer.h"
#include "RenderManager.h"
#include "DebugManager.h"
#include "FileSystemManager.h"
#include "KeyInputManager.h"
#include "ResourceManager.h"
#include "SaveDataManager.h"
#include "SceneManager.h"
#include "SoundManager.h"

namespace planeta_engine {
	using namespace core;
	class PlanetaEngine::Impl_ {
	private:
		std::list<std::function<void()>> finalize_handls_;
		std::unique_ptr<SceneManager> scene_manager_;
	public:
		Impl_() :scene_manager_(std::make_unique<SceneManager>()) {}
		//エンジンの初期化
		bool InitializeSubSystems() {
			//////////////////////////////////////////////////////////////////////////
			//システムタイマーの初期化
			//////////////////////////////////////////////////////////////////////////
			if (debug::SystemTimer::instance().Initialize()) { finalize_handls_.push_front([] {debug::SystemTimer::instance().Finalize(); }); }
			else { assert(false); return false; }
			//////////////////////////////////////////////////////////////////////////
			//システムログの初期化
			//////////////////////////////////////////////////////////////////////////
			{
				auto ret = init_funcs::InitializeLogSystem();
				if (std::get<0>(ret) == false) { return false; } 
				else { finalize_handls_.push_front(std::get<1>(ret)); }
			}
			//////////////////////////////////////////////////////////////////////////
			//ファイルシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			FileSystemManager& flm = FileSystemManager::instance();
			//リソース用ファイルアクセサ設定
			auto resource_file_accesor = init_funcs::CreateFileAccessor(init_funcs::FileAccessorKind::Resource);
			//SaveData用ファイルアクセサ設定
			auto savedata_dir_accesor = init_funcs::CreateFileAccessor(init_funcs::FileAccessorKind::SaveData);
			//system用ファイルアクセサ設定
			auto system_dir_accesor = init_funcs::CreateFileAccessor(init_funcs::FileAccessorKind::System);
			//config用ファイルアクセサ設定
			auto config_dir_accesor = init_funcs::CreateFileAccessor(init_funcs::FileAccessorKind::Config);
			if(flm.Initialize()){ finalize_handls_.push_front([] {FileSystemManager::instance().Finalize(); }); }
			else { PE_LOG_FATAL("ファイルシステムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//エンジン設定の読み込み
			//////////////////////////////////////////////////////////////////////////
			if (!init_funcs::LoadEngineConfig(system_dir_accesor)) { return false; }
			//////////////////////////////////////////////////////////////////////////
			//リソースシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			{
				auto ret = init_funcs::InitializeResourceSystem(resource_file_accesor);
				if (std::get<0>(ret) == false) { return false; }
				else { finalize_handls_.push_front(std::get<1>(ret)); }
			}
			//////////////////////////////////////////////////////////////////////////
			//セーブデータシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			SaveDataManager::instance().SetFileAccessor_(savedata_dir_accesor);
			if(SaveDataManager::instance().Initialize()){ finalize_handls_.push_front([] {SaveDataManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("セーブデータシステムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//DXライブラリの初期化
			//////////////////////////////////////////////////////////////////////////
			{
				auto ret = init_funcs::InitializeDxLib();
				if (std::get<0>(ret) == false) { return false; } 
				else { finalize_handls_.push_front(std::get<1>(ret)); }
			}
			//////////////////////////////////////////////////////////////////////////
			//描画システムの初期化
			//////////////////////////////////////////////////////////////////////////
			if(core::RenderManager::instance().Initialize()){ finalize_handls_.push_front([] {core::RenderManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("描画システムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//サウンドシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			if(core::SoundManager::instance().Initialize()){ finalize_handls_.push_front([] {core::SoundManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("サウンドシステムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//入力システムの初期化
			//////////////////////////////////////////////////////////////////////////
			//キーコンフィグデータのセット予定
			if(core::KeyInputManager::instance().Initialize()){ finalize_handls_.push_front([] {core::KeyInputManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("入力システムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//デバッグシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			if(debug::DebugManager::instance().Initialize()){ finalize_handls_.push_front([] {debug::DebugManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("デバッグシステムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//プログラム用定義の読み込み
			//////////////////////////////////////////////////////////////////////////
			core::ProgramDefinitionData pdd;
			if (!init_funcs::LoadProgramDefinition(system_dir_accesor, &pdd)) {
				PE_LOG_ERROR("プログラム用定義ファイルの読み込みに失敗しました。");
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//シーンシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			//衝突マトリックスの設定
			scene_manager_->SetCollisionGroupMatrix_(std::make_shared<CollisionGroupMatrix>(std::move(pdd.collision_group_matrix)));
			if (scene_manager_->Initialize()) { finalize_handls_.push_front([&srn_mgr = *scene_manager_] {srn_mgr.Finalize(); }); }
			else { PE_LOG_FATAL("シーンシステムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//ゲームの開始準備
			//////////////////////////////////////////////////////////////////////////
			scene_manager_->LoadAndTransitionScene(pdd.startup_scene_id); //スタートアップシーンの開始依頼

			return true;

		}
		//エンジンの終了処理
		void FinalizeSubSystems() {
			for (auto&& fh : finalize_handls_) {
				fh();
			}
		}
		//エンジンの更新
		GameStatus UpdateSubSystems() {
			if (ProcessMessage() < 0) { return GameStatus::Quit; } //DXライブラリの更新
			KeyInputManager::instance().Update(); //キー入力の更新
			auto sst = scene_manager_->Process_(); //シーンの更新
			RenderManager::instance().Update(); //描画システムの更新
			SoundManager::instance().Update(); //サウンドシステムの更新
			debug::SystemTimer::instance().IncrementFrameCount(); //フレームカウントのインクリメント
			
			switch (sst) {
			case planeta_engine::core::SceneManager::SceneStatus_::Continue:
				return GameStatus::Continue;
			case planeta_engine::core::SceneManager::SceneStatus_::Quit:
				return GameStatus::Quit;
			case planeta_engine::core::SceneManager::SceneStatus_::Error:
				return GameStatus::Error;
			default:
				assert(false);
				return GameStatus::Error;
			}
		}
	};

	PlanetaEngine::PlanetaEngine() :is_initialized_(false), impl_(std::make_unique<Impl_>()) {

	}

	PlanetaEngine::~PlanetaEngine() {
		assert(!is_initialized_);
	}

	bool PlanetaEngine::Initialize() {
		if (is_initialized_) { assert(false); return false; }
		if (impl_->InitializeSubSystems()) {
			PE_LOG_MESSAGE("PlanetaEngineが正常に初期化されました。");
			is_initialized_ = true;
			return true;
		} else {
			PE_LOG_FATAL("PlanetaEngineの初期化に失敗しました。");
			impl_->FinalizeSubSystems();
			return false;
		}
	}

	void PlanetaEngine::Finalize() {
		if (!is_initialized_) { assert(false); return; }
		impl_->FinalizeSubSystems();
		PE_LOG_MESSAGE("PlanetaEngineを終了しました。");
	}

	GameStatus PlanetaEngine::Update() {
		return impl_->UpdateSubSystems();
	}
}