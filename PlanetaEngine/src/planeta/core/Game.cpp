#include <cassert>
#include <functional>
#include <list>

#include "DxLib.h"

#include "Game.hpp"

#include "ResourceManager.hpp"
#include "LogManager.hpp"
#include "SceneManager.hpp"
#include "InputManager.hpp"
#include "PerfoamanceManager.hpp"
#include "RenderingManager.hpp"
#include "SoundManager.hpp"
#include "SaveManager.hpp"
#include "DebugManager.hpp"
#include "ConfigManager.hpp"

#include "LogUtility.hpp"

#include "InitFunctions.hpp"
#include "SystemVariables.hpp"


#include "planeta/reflection/Reflection.hpp"

namespace plnt {
	using namespace private_;
	class Game::Impl_ {
	private:
		std::list<std::function<void()>> finalize_handls_;
	public:
		std::shared_ptr<ResourceManager> resource_manager;
		std::shared_ptr<LogManager> log_manager;
		std::shared_ptr<SceneManager> scene_manager;
		std::shared_ptr<InputManager> input_manager;
		std::shared_ptr<PerformanceManager> performance_manager;
		std::shared_ptr<RenderingManager> rendering_manager;
		std::shared_ptr<SoundManager> sound_manager;
		std::shared_ptr<SaveManager> save_manager;
		std::shared_ptr<DebugManager> debug_manager;
		std::shared_ptr<ConfigManager> config_manager;

		Impl_() {}
		bool is_initialized = false;
		//エンジンの初期化
		bool InitializeSubSystems() {
			//////////////////////////////////////////////////////////////////////////
			//システムタイマーの初期化
			//////////////////////////////////////////////////////////////////////////
			if (performance_manager == nullptr) {
				PE_LOG_FATAL("パフォーマンスマネージャが設定されていません。");
				return false;
			}
			if (performance_manager->Initialize()) { finalize_handls_.push_front([this] {performance_manager->Finalize(); }); }
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
				reflection::Reflection::Initialize();
				PE_LOG_MESSAGE("リフレクションシステムが有効です。", reflection::Reflection::GetRegisteredClassCount(),"個のクラスが登録されています。");
			} 
			catch (reflection::reflection_error& e) {
				PE_LOG_FATAL("リフレクションシステムの初期化に失敗しました。:", e.what());
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//ファイルシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			//リソース用ファイルアクセサ設定
			auto resource_file_manipulator = init_funcs::CreateFileManipurator(init_funcs::FileAccessorKind::Resource);
			if (resource_file_manipulator == nullptr) { return false; }
			//SaveData用ファイルアクセサ設定
			auto savedata_dir_manipulator = init_funcs::CreateFileManipurator(init_funcs::FileAccessorKind::SaveData);
			if (savedata_dir_manipulator == nullptr) { return false; }
			//system用ファイルアクセサ設定
			auto system_dir_manipulator = init_funcs::CreateFileManipurator(init_funcs::FileAccessorKind::System);
			if (system_dir_manipulator == nullptr) { return false; }
			//config用ファイルアクセサ設定
			auto config_dir_manipulator = init_funcs::CreateFileManipurator(init_funcs::FileAccessorKind::Config);
			if (config_dir_manipulator == nullptr) { return false; }
			//////////////////////////////////////////////////////////////////////////
			//設定の読み込み
			//////////////////////////////////////////////////////////////////////////
			if (resource_manager == nullptr) {
				PE_LOG_FATAL("コンフィグマネージャが設定されていません。");
				return false;
			}
			if (!init_funcs::LoadConfig(*config_manager, system_dir_manipulator, config_dir_manipulator)) { 
				PE_LOG_FATAL("設定ファイルの読み込みに失敗しました。");
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//リソースマネージャの初期化
			//////////////////////////////////////////////////////////////////////////
			{
				if (resource_manager == nullptr) {
					PE_LOG_FATAL("リソースマネージャが設定されていません。");
					return false;
				}
				auto ret = init_funcs::InitializeResourceSystem(*resource_manager, resource_file_manipulator);
				if (std::get<0>(ret) == false) { return false; }
				else { finalize_handls_.push_front(std::get<1>(ret)); }
			}
			//////////////////////////////////////////////////////////////////////////
			//セーブデータシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			if (save_manager == nullptr) {
				PE_LOG_FATAL("セーブマネージャが設定されていません。");
				return false;
			}
			save_manager->SetFileManipurator_(savedata_dir_manipulator);
			if(save_manager->Initialize()){ finalize_handls_.push_front([this] {save_manager->Finalize(); }); }
			else{ PE_LOG_FATAL("セーブデータシステムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//DXライブラリの初期化
			//////////////////////////////////////////////////////////////////////////
			{
				auto ret = init_funcs::InitializeDxLib(*config_manager);
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
			if (rendering_manager == nullptr) {
				PE_LOG_FATAL("レンダリングマネージャが設定されていません。");
				return false;
			}
			if(rendering_manager->Initialize()){ finalize_handls_.push_front([this] {rendering_manager->Finalize(); }); }
			else{ PE_LOG_FATAL("描画システムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//サウンドシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			if (sound_manager == nullptr) {
				PE_LOG_FATAL("サウンドマネージャが設定されていません。");
				return false;
			}
			if(sound_manager->Initialize()){ finalize_handls_.push_front([this] {sound_manager->Finalize(); }); }
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
			if (debug_manager == nullptr) {
				PE_LOG_FATAL("デバッグマネージャが設定されていません。");
				return false;
			}
			if(debug_manager->Initialize(*rendering_manager)){ finalize_handls_.push_front([this] {debug_manager->Finalize(); }); }
			else{ PE_LOG_FATAL("デバッグシステムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//シーンシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			if (scene_manager == nullptr) {
				PE_LOG_FATAL("シーンマネージャが設定されていません。");
				return false;
			}
			//リソースマネージャのセット
			scene_manager->SetResouceManager(resource_manager);
			if (scene_manager->Initialize()) { finalize_handls_.push_front([this] {scene_manager->Finalize(); }); }
			else { PE_LOG_FATAL("シーンシステムの初期化に失敗しました。"); return false; }
			//////////////////////////////////////////////////////////////////////////
			//ゲームの開始準備
			//////////////////////////////////////////////////////////////////////////
			scene_manager->LoadAndTransitionScene(config_manager->startup_scene_id()); //スタートアップシーンの開始依頼

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
			debug_manager->PreRenderingUpdate(); //デバッグマネージャの描画前更新
			rendering_manager->Update(); //描画システムの更新
			sound_manager->Update(); //サウンドシステムの更新
			performance_manager->Update(); //パフォーマンスマネージャの更新
			debug_manager->PostRenderingUpdate(); //デバッグマネージャの描画後更新

			switch (sst) {
			case plnt::private_::SceneStatus_::Continue:
				return GameStatus::Continue;
			case plnt::private_::SceneStatus_::Quit:
				return GameStatus::Quit;
			case plnt::private_::SceneStatus_::Error:
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

	std::shared_ptr<plnt::IResourceManager> Game::resource_manager()const {
		return impl_->resource_manager;
	}

	void Game::SetLogManager(const std::shared_ptr<private_::LogManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->log_manager = mgr;
	}

	std::shared_ptr<plnt::ILogManager> Game::log_manager() const {
		return impl_->log_manager;
	}

	void Game::SetSceneManager(const std::shared_ptr<private_::SceneManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->scene_manager = mgr;
	}

	std::shared_ptr<plnt::ISceneManager> Game::scene_manager() const {
		return impl_->scene_manager;
	}

	void Game::SetInputManager(const std::shared_ptr<private_::InputManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->input_manager = mgr;
	}

	std::shared_ptr<plnt::IInputManager> Game::input_manager() const {
		return impl_->input_manager;
	}

	void Game::SetPerformanceManager(const std::shared_ptr<private_::PerformanceManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->performance_manager = mgr;
	}

	std::shared_ptr<plnt::IPerformanceManager> Game::performance_manager() const {
		return impl_->performance_manager;
	}

	void Game::SetRenderingManager(const std::shared_ptr<private_::RenderingManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->rendering_manager = mgr;
	}

	std::shared_ptr<plnt::IRenderingManager> Game::rendering_manager() const {
		return impl_->rendering_manager;
	}

	void Game::SetSoundManager(const std::shared_ptr<private_::SoundManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->sound_manager = mgr;
	}

	std::shared_ptr<plnt::ISoundManager> Game::sound_manager() const {
		return impl_->sound_manager;
	}

	void Game::SetSaveManager(const std::shared_ptr<private_::SaveManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->save_manager = mgr;
	}

	std::shared_ptr<plnt::ISaveManager> Game::save_manager() const {
		return impl_->save_manager;
	}

	void Game::SetDebugManager(const std::shared_ptr<private_::DebugManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->debug_manager = mgr;
	}

	std::shared_ptr<plnt::IDebugManager> Game::debug_manager() const {
		return impl_->debug_manager;
	}

	void Game::SetConfigManager(const std::shared_ptr<private_::ConfigManager>& mgr) {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->config_manager = mgr;
	}

	std::shared_ptr<plnt::IConfigManager> Game::config_manager() const {
		return impl_->config_manager;
	}

}