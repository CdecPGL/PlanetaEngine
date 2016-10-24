#include <cassert>
#include <functional>
#include <list>

#include "DxLib.h"

#include "Game.h"

#include "ResourceManager.h"
#include "LogManager.h"
#include "FileSystemManager.h"
#include "SceneManager.h"
#include "InputManager.h"

#include "LogUtility.h"

#include "InitFunctions.h"
#include "ProgramDefinitionData.h"
#include "SystemVariables.h"


#include "Reflection.h"

#include "SystemTimer.h"
#include "RenderManager.h"
#include "DebugManager.h"
#include "SaveDataManager.h"
#include "SoundManager.h"

namespace planeta {
	using namespace private_;
	class Game::Impl_ {
	private:
		std::list<std::function<void()>> finalize_handls_;
	public:
		std::shared_ptr<ResourceManager> resource_manager;
		std::shared_ptr<LogManager> log_manager;
		std::shared_ptr<FileSystemManager> file_system_manager;
		std::shared_ptr<SceneManager> scene_manager;
		std::shared_ptr<InputManager> input_manager;

		Impl_() {}
		bool is_initialized = false;
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
				if (log_manager == nullptr) {
					PE_LOG_FATAL("ログマネージャが設定されていません。");
					return false;
				}
				if (log_manager->Initialize()) {
					finalize_handls_.push_front([this] {log_manager->Finalize(); });
				} else {
					PE_LOG_FATAL("ログシステムの初期化に失敗しました。");
					return false;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			//リフレクションシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			try {
				Reflection::Initialize();
				PE_LOG_MESSAGE("リフレクションシステムが有効です。", Reflection::GetRegisteredClassCount(),"個のクラスが登録されています。");
			} 
			catch (reflection_error& e) {
				PE_LOG_FATAL("リフレクションシステムの初期化に失敗しました。:", e.what());
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//ファイルシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			if (file_system_manager == nullptr) {
				PE_LOG_FATAL("ファイルシステムマネージャが設定されていません。");
				return false;
			}
			FileSystemManager& flm = *file_system_manager;
			if(flm.Initialize()){ finalize_handls_.push_front([this] {file_system_manager->Finalize(); }); }
			else { PE_LOG_FATAL("ファイルシステムの初期化に失敗しました。"); return false; }
			//リソース用ファイルアクセサ設定
			auto resource_file_accesor = init_funcs::CreateFileAccessor(flm, init_funcs::FileAccessorKind::Resource);
			if (resource_file_accesor == nullptr) { return false; }
			//SaveData用ファイルアクセサ設定
			auto savedata_dir_accesor = init_funcs::CreateFileAccessor(flm, init_funcs::FileAccessorKind::SaveData);
			if (savedata_dir_accesor == nullptr) { return false; }
			//system用ファイルアクセサ設定
			auto system_dir_accesor = init_funcs::CreateFileAccessor(flm, init_funcs::FileAccessorKind::System);
			if (system_dir_accesor == nullptr) { return false; }
			//config用ファイルアクセサ設定
			auto config_dir_accesor = init_funcs::CreateFileAccessor(flm, init_funcs::FileAccessorKind::Config);
			if (config_dir_accesor == nullptr) { return false; }
			//////////////////////////////////////////////////////////////////////////
			//エンジン設定の読み込み
			//////////////////////////////////////////////////////////////////////////
			if (!init_funcs::LoadEngineConfig(system_dir_accesor)) { return false; }
			//////////////////////////////////////////////////////////////////////////
			//リソースマネージャの初期化
			//////////////////////////////////////////////////////////////////////////
			{
				if (resource_manager == nullptr) {
					PE_LOG_FATAL("リソースマネージャが設定されていません。");
					return false;
				}
				auto ret = init_funcs::InitializeResourceSystem(*resource_manager, resource_file_accesor);
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
			//Effekseerの初期化
			//////////////////////////////////////////////////////////////////////////
			{
				auto ret = init_funcs::InitializeEffekseer();
				if (std::get<0>(ret) == false) { return false; }
				else { finalize_handls_.push_front(std::get<1>(ret)); }
			}
			//////////////////////////////////////////////////////////////////////////
			//描画システムの初期化
			//////////////////////////////////////////////////////////////////////////
			if(private_::RenderManager::instance().Initialize()){ finalize_handls_.push_front([] {private_::RenderManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("描画システムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//サウンドシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			if(private_::SoundManager::instance().Initialize()){ finalize_handls_.push_front([] {private_::SoundManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("サウンドシステムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//入力システムの初期化
			//////////////////////////////////////////////////////////////////////////
			if (input_manager == nullptr) {
				PE_LOG_FATAL("インプットマネージャが設定されていません。");
				return false;
			}
			//キーコンフィグデータのセット予定
			if(input_manager->Initialize()){ finalize_handls_.push_front([this] {input_manager->Finalize(); }); }
			else{ PE_LOG_FATAL("入力システムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//デバッグシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			if(debug::DebugManager::instance().Initialize()){ finalize_handls_.push_front([] {debug::DebugManager::instance().Finalize(); }); }
			else{ PE_LOG_FATAL("デバッグシステムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//プログラム用定義の読み込み
			//////////////////////////////////////////////////////////////////////////
			private_::ProgramDefinitionData pdd;
			if (!init_funcs::LoadProgramDefinition(system_dir_accesor, &pdd)) {
				PE_LOG_ERROR("プログラム用定義ファイルの読み込みに失敗しました。");
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//シーンシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			if (scene_manager == nullptr) {
				PE_LOG_FATAL("シーンマネージャが設定されていません。");
				return false;
			}
			//リソースマネージャのセット
			scene_manager->SetResouceManager(resource_manager);
			//衝突マトリックスの設定
			scene_manager->SetCollisionGroupMatrix_(std::make_shared<CollisionGroupMatrix>(std::move(pdd.collision_group_matrix)));
			if (scene_manager->Initialize()) { finalize_handls_.push_front([this] {scene_manager->Finalize(); }); }
			else { PE_LOG_FATAL("シーンシステムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//キャッシュなどの削除
			//////////////////////////////////////////////////////////////////////////
			flm.DisposeFileAccessor(system_variables::file_system::SystemFileAccessorID); //システムファイルアクセサ削除
			flm.DeleteCache(); //キャッシュ削除
			//////////////////////////////////////////////////////////////////////////
			//ゲームの開始準備
			//////////////////////////////////////////////////////////////////////////
			scene_manager->LoadAndTransitionScene(pdd.startup_scene_id); //スタートアップシーンの開始依頼

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
			input_manager->Update(); //入力の更新
			auto sst = scene_manager->Process_(); //シーンの更新
			RenderManager::instance().Update(); //描画システムの更新
			SoundManager::instance().Update(); //サウンドシステムの更新
			debug::SystemTimer::instance().IncrementFrameCount(); //フレームカウントのインクリメント
			
			switch (sst) {
			case planeta::private_::SceneStatus_::Continue:
				return GameStatus::Continue;
			case planeta::private_::SceneStatus_::Quit:
				return GameStatus::Quit;
			case planeta::private_::SceneStatus_::Error:
				return GameStatus::Error;
			default:
				assert(false);
				return GameStatus::Error;
			}
		}
	};

	Game::Game() :impl_(std::make_unique<Impl_>()) {

	}

	Game::~Game() {
		assert(impl_->is_initialized == false);
	}

	bool Game::Initialize() {
		if (impl_->is_initialized) { assert(false); return false; }
		if (impl_->InitializeSubSystems()) {
			PE_LOG_MESSAGE("PlanetaEngineが正常に初期化されました。");
			impl_->is_initialized = true;
			return true;
		} else {
			PE_LOG_FATAL("PlanetaEngineの初期化に失敗しました。");
			impl_->FinalizeSubSystems();
			return false;
		}
	}

	void Game::Finalize() {
		if (impl_->is_initialized==false) { assert(false); return; }
		PE_LOG_MESSAGE("PlanetaEngineを終了します。");
		impl_->FinalizeSubSystems();
		impl_->is_initialized = false;
	}

	GameStatus Game::Update() {
		return impl_->UpdateSubSystems();
	}

	bool Game::is_initialized() const {
		return impl_->is_initialized;
	}

	void Game::SetResourceManager(const std::shared_ptr<private_::ResourceManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->resource_manager = mgr;
	}

	std::shared_ptr<planeta::IResourceManager> Game::resource_manager()const {
		return impl_->resource_manager;
	}

	void Game::SetLogManager(const std::shared_ptr<private_::LogManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->log_manager = mgr;
	}

	std::shared_ptr<planeta::ILogManager> Game::log_manager() const {
		return impl_->log_manager;
	}

	void Game::SetFileSystemManager(const std::shared_ptr<private_::FileSystemManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->file_system_manager = mgr;
	}

	std::shared_ptr<planeta::IFileSystemManager> Game::file_system_manager() const {
		return impl_->file_system_manager;
	}

	void Game::SetSceneManager(const std::shared_ptr<private_::SceneManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->scene_manager = mgr;
	}

	std::shared_ptr<planeta::ISceneManager> Game::scene_manager() const {
		return impl_->scene_manager;
	}

	void Game::SetInputManager(const std::shared_ptr<private_::InputManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->input_manager = mgr;
	}

	std::shared_ptr<planeta::IInputManager> Game::input_manager() const {
		return impl_->input_manager;
	}

}