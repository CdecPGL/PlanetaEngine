#include <cassert>
#include <functional>
#include <list>

#include "DxLib.h"

#include "game.hpp"
#include "resource_manager.hpp"
#include "log_manager.hpp"
#include "scene_manager.hpp"
#include "input_manager.hpp"
#include "perfoamance_manager.hpp"
#include "rendering_manager.hpp"
#include "sound_manager.hpp"
#include "save_manager.hpp"
#include "debug_manager.hpp"
#include "config_manager.hpp"
#include "log_utility.hpp"
#include "init_functions.hpp"
#include "SystemVariables.hpp"


#include "planeta/reflection/reflection.hpp"

namespace plnt {
	using namespace private_;

	class game::impl {
		std::list<std::function<void()>> finalize_handles_;

	public:
		std::shared_ptr<private_::resource_manager> resource_manager;
		std::shared_ptr<private_::log_manager> log_manager;
		std::shared_ptr<private_::scene_manager> scene_manager;
		std::shared_ptr<private_::input_manager> input_manager;
		std::shared_ptr<private_::performance_manager> performance_manager;
		std::shared_ptr<private_::rendering_manager> rendering_manager;
		std::shared_ptr<private_::sound_manager> sound_manager;
		std::shared_ptr<private_::save_manager> save_manager;
		std::shared_ptr<private_::debug_manager> debug_manager;
		std::shared_ptr<private_::config_manager> config_manager;

		impl() = default;

		bool is_initialized = false;
		//エンジンの初期化
		bool initialize_sub_systems() {
			//////////////////////////////////////////////////////////////////////////
			//システムタイマーの初期化
			//////////////////////////////////////////////////////////////////////////
			if (performance_manager == nullptr) {
				PE_LOG_FATAL("パフォーマンスマネージャが設定されていません。");
				return false;
			}
			if (performance_manager->initialize()) {
				finalize_handles_.push_front([this] { performance_manager->finalize(); });
			} else {
				assert(false);
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//システムログの初期化
			//////////////////////////////////////////////////////////////////////////
			{
				if (log_manager == nullptr) {
					PE_LOG_FATAL("ログマネージャが設定されていません。");
					return false;
				}
				if (log_manager->
					initialize()) { finalize_handles_.push_front([this] { log_manager->finalize(); }); } else {
					PE_LOG_FATAL("ログシステムの初期化に失敗しました。");
					return false;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			//リフレクションシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			try {
				reflection::reflection::initialize();
				PE_LOG_MESSAGE("リフレクションシステムが有効です。", reflection::reflection::get_registered_class_count(),
				               "個のクラスが登録されています。");
			} catch (reflection::reflection_error &e) {
				PE_LOG_FATAL("リフレクションシステムの初期化に失敗しました。:", e.what());
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//ファイルシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			//リソース用ファイルアクセサ設定
			const auto resource_file_manipulator = create_file_manipulator(init_funcs::file_accessor_kind::resource);
			if (resource_file_manipulator == nullptr) { return false; }
			//SaveData用ファイルアクセサ設定
			const auto save_data_dir_manipulator = create_file_manipulator(init_funcs::file_accessor_kind::save_data);
			if (save_data_dir_manipulator == nullptr) { return false; }
			//system用ファイルアクセサ設定
			const auto system_dir_manipulator = create_file_manipulator(init_funcs::file_accessor_kind::system);
			if (system_dir_manipulator == nullptr) { return false; }
			//config用ファイルアクセサ設定
			const auto config_dir_manipulator = create_file_manipulator(init_funcs::file_accessor_kind::config);
			if (config_dir_manipulator == nullptr) { return false; }
			//////////////////////////////////////////////////////////////////////////
			//設定の読み込み
			//////////////////////////////////////////////////////////////////////////
			if (resource_manager == nullptr) {
				PE_LOG_FATAL("コンフィグマネージャが設定されていません。");
				return false;
			}
			if (!init_funcs::load_config(*config_manager, system_dir_manipulator, config_dir_manipulator)) {
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
				if (const auto ret = init_funcs::initialize_resource_system(
						*resource_manager, resource_file_manipulator);
					std::get<0>(ret) == false) { return false; } else {
					finalize_handles_.push_front(std::get<1>(ret));
				}
			}
			//////////////////////////////////////////////////////////////////////////
			//セーブデータシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			if (save_manager == nullptr) {
				PE_LOG_FATAL("セーブマネージャが設定されていません。");
				return false;
			}
			save_manager->set_file_manipulator(save_data_dir_manipulator);
			if (save_manager->
				initialize()) { finalize_handles_.push_front([this] { save_manager->finalize(); }); } else {
				PE_LOG_FATAL("セーブデータシステムの初期化に失敗しました。");
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//DXライブラリの初期化
			//////////////////////////////////////////////////////////////////////////
			{
				if (const auto ret = init_funcs::initialize_dx_lib(*config_manager); std::get<0>(ret) == false) {
					return false;
				} else { finalize_handles_.push_front(std::get<1>(ret)); }
			}
			//////////////////////////////////////////////////////////////////////////
			//Effekseerの初期化
			//////////////////////////////////////////////////////////////////////////
			{
				if (const auto ret = init_funcs::initialize_effekseer(); std::get<0>(ret) ==
					false) { return false; } else { finalize_handles_.push_front(std::get<1>(ret)); }
			}
			//////////////////////////////////////////////////////////////////////////
			//描画システムの初期化
			//////////////////////////////////////////////////////////////////////////
			if (rendering_manager == nullptr) {
				PE_LOG_FATAL("レンダリングマネージャが設定されていません。");
				return false;
			}
			if (rendering_manager->initialize()) {
				finalize_handles_.push_front([this] { rendering_manager->finalize(); });
			} else {
				PE_LOG_FATAL("描画システムの初期化に失敗しました。");
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//サウンドシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			if (sound_manager == nullptr) {
				PE_LOG_FATAL("サウンドマネージャが設定されていません。");
				return false;
			}
			if (sound_manager->
				initialize()) { finalize_handles_.push_front([this] { sound_manager->finalize(); }); } else {
				PE_LOG_FATAL("サウンドシステムの初期化に失敗しました。");
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//入力システムの初期化
			//////////////////////////////////////////////////////////////////////////
			if (input_manager == nullptr) {
				PE_LOG_FATAL("インプットマネージャが設定されていません。");
				return false;
			}
			//キーコンフィグデータのセット予定
			if (input_manager->
				initialize()) { finalize_handles_.push_front([this] { input_manager->finalize(); }); } else {
				PE_LOG_FATAL("入力システムの初期化に失敗しました。");
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//デバッグシステムの初期化
			//////////////////////////////////////////////////////////////////////////
			if (debug_manager == nullptr) {
				PE_LOG_FATAL("デバッグマネージャが設定されていません。");
				return false;
			}
			if (debug_manager->initialize(*rendering_manager)) {
				finalize_handles_.push_front([this] { debug_manager->finalize(); });
			} else {
				PE_LOG_FATAL("デバッグシステムの初期化に失敗しました。");
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
			scene_manager->set_resource_manager(resource_manager);
			if (scene_manager->
				initialize()) { finalize_handles_.push_front([this] { scene_manager->finalize(); }); } else {
				PE_LOG_FATAL("シーンシステムの初期化に失敗しました。");
				return false;
			}
			//////////////////////////////////////////////////////////////////////////
			//ゲームの開始準備
			//////////////////////////////////////////////////////////////////////////
			scene_manager->load_and_transition_scene(config_manager->startup_scene_id()); //スタートアップシーンの開始依頼

			return true;
		}

		//エンジンの終了処理
		void finalize_sub_systems() const { for (auto &&fh : finalize_handles_) { fh(); } }

		//エンジンの更新
		[[nodiscard]] game_status update_sub_systems() const {
			if (ProcessMessage() < 0) { return game_status::quit; } //DXライブラリの更新
			input_manager->update(); //入力の更新
			const auto sst = scene_manager->process(); //シーンの更新
			debug_manager->pre_rendering_update(); //デバッグマネージャの描画前更新
			rendering_manager->update(); //描画システムの更新
			sound_manager->update(); //サウンドシステムの更新
			performance_manager->update(); //パフォーマンスマネージャの更新
			debug_manager->post_rendering_update(); //デバッグマネージャの描画後更新

			switch (sst) {
				case scene_status::play:
					return game_status::play;
				case scene_status::quit:
					return game_status::quit;
				case scene_status::error:
					return game_status::error;
			}

			assert(false);
			return game_status::error;
		}
	};

	game::game() : impl_(std::make_unique<impl>()) {}

	game::~game() { assert(impl_->is_initialized == false); }

	bool game::initialize() {
		if (impl_->is_initialized) {
			assert(false);
			return false;
		}
		if (impl_->initialize_sub_systems()) {
			PE_LOG_MESSAGE("PlanetaEngineが正常に初期化されました。");
			impl_->is_initialized = true;
			return true;
		}
		PE_LOG_FATAL("PlanetaEngineの初期化に失敗しました。");
		impl_->finalize_sub_systems();
		return false;
	}

	void game::finalize() {
		if (impl_->is_initialized == false) {
			assert(false);
			return;
		}
		PE_LOG_MESSAGE("PlanetaEngineを終了します。");
		impl_->finalize_sub_systems();
		impl_->is_initialized = false;
	}

	game_status game::update() const { return impl_->update_sub_systems(); }

	bool game::is_initialized() const { return impl_->is_initialized; }

	void game::set_resource_manager(const std::shared_ptr<private_::resource_manager> &mgr) const {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->resource_manager = mgr;
	}

	std::shared_ptr<i_resource_manager> game::resource_manager() const { return impl_->resource_manager; }

	void game::set_log_manager(const std::shared_ptr<private_::log_manager> &mgr) const {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->log_manager = mgr;
	}

	std::shared_ptr<i_log_manager> game::log_manager() const { return impl_->log_manager; }

	void game::set_scene_manager(const std::shared_ptr<private_::scene_manager> &mgr) const {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->scene_manager = mgr;
	}

	std::shared_ptr<i_scene_manager> game::scene_manager() const { return impl_->scene_manager; }

	void game::set_input_manager(const std::shared_ptr<private_::input_manager> &mgr) const {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->input_manager = mgr;
	}

	std::shared_ptr<i_input_manager> game::input_manager() const { return impl_->input_manager; }

	void game::set_performance_manager(const std::shared_ptr<private_::performance_manager> &mgr) const {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->performance_manager = mgr;
	}

	std::shared_ptr<i_performance_manager> game::performance_manager() const { return impl_->performance_manager; }

	void game::set_rendering_manager(const std::shared_ptr<private_::rendering_manager> &mgr) const {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->rendering_manager = mgr;
	}

	std::shared_ptr<i_rendering_manager> game::rendering_manager() const { return impl_->rendering_manager; }

	void game::set_sound_manager(const std::shared_ptr<private_::sound_manager> &mgr) const {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->sound_manager = mgr;
	}

	std::shared_ptr<i_sound_manager> game::sound_manager() const { return impl_->sound_manager; }

	void game::set_save_manager(const std::shared_ptr<private_::save_manager> &mgr) const {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->save_manager = mgr;
	}

	std::shared_ptr<i_save_manager> game::save_manager() const { return impl_->save_manager; }

	void game::set_debug_manager(const std::shared_ptr<private_::debug_manager> &mgr) const {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->debug_manager = mgr;
	}

	std::shared_ptr<i_debug_manager> game::debug_manager() const { return impl_->debug_manager; }

	void game::set_config_manager(const std::shared_ptr<private_::config_manager> &mgr) const {
		if (is_initialized()) {
			PE_LOG_ERROR("マネージャの設定はPlanetaEngine初期化前に行わなければなりません。");
			return;
		}
		impl_->config_manager = mgr;
	}

	std::shared_ptr<i_config_manager> game::config_manager() const { return impl_->config_manager; }
}
