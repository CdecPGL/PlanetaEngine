#pragma once

#include <memory>

#include "singleton_template.hpp"

namespace plnt {
	class i_resource_manager;
	class i_log_manager;
	class i_scene_manager;
	class i_input_manager;
	class i_performance_manager;
	class i_rendering_manager;
	class i_sound_manager;
	class i_save_manager;
	class i_debug_manager;
	class i_config_manager;

	namespace private_ {
		class resource_manager;
		class log_manager;
		class scene_manager;
		class input_manager;
		class performance_manager;
		class rendering_manager;
		class sound_manager;
		class save_manager;
		class debug_manager;
		class config_manager;
	}

	/*! ゲームのステータス*/
	enum class game_status { play, quit, error };

	/*! PlanetaEngineクラス*/
	class game final : public util::singleton_template<game> {
		friend singleton_template;

	public:
		/*! 標準のマネージャを設定する。初期化前に実行しなければならない*/
		void set_standard_managers() const;
		/*! エンジンの初期化*/
		bool initialize() override;
		/*! エンジンの終了処理*/
		void finalize() override;
		/*! エンジンの更新(初期化が正常に行われていない状態での呼び出しは未定義動作)*/
		[[nodiscard]] game_status update() const;
		/*! 初期化されているか*/
		[[nodiscard]] bool is_initialized() const;
		/*! リソースマネージャを設定する(初期化前)*/
		void set_resource_manager(const std::shared_ptr<private_::resource_manager> &mgr) const;
		/*! ログマネージャを設定する(初期化前)*/
		auto set_log_manager(const std::shared_ptr<private_::log_manager> &mgr) const -> void;
		/*! シーンマネージャを設定する(初期化前)*/
		void set_scene_manager(const std::shared_ptr<private_::scene_manager> &mgr) const;
		/*! インプットマネージャを設定する(初期化前)*/
		void set_input_manager(const std::shared_ptr<private_::input_manager> &mgr) const;
		/*! パフォーマンスマネージャを設定する(初期化前)*/
		void set_performance_manager(const std::shared_ptr<private_::performance_manager> &mgr) const;
		/*! レンダリングマネージャを設定する(初期化前)*/
		void set_rendering_manager(const std::shared_ptr<private_::rendering_manager> &mgr) const;
		/*! サウンドマネージャを設定する(初期化前)*/
		void set_sound_manager(const std::shared_ptr<private_::sound_manager> &mgr) const;
		/*! セーブマネージャを設定する(初期化前)*/
		void set_save_manager(const std::shared_ptr<private_::save_manager> &mgr) const;
		/*! デバッグマネージャを設定する(初期化前)*/
		void set_debug_manager(const std::shared_ptr<private_::debug_manager> &mgr) const;
		/*! コンフィグマネージャを設定する(初期化前)*/
		void set_config_manager(const std::shared_ptr<private_::config_manager> &mgr) const;
		/*! リソースマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<i_resource_manager> resource_manager() const;
		/*! ログマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<i_log_manager> log_manager() const;
		/*! シーンマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<i_scene_manager> scene_manager() const;
		/*! インプットマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<i_input_manager> input_manager() const;
		/*! パフォーマンスマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<i_performance_manager> performance_manager() const;
		/*! レンダリングマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<i_rendering_manager> rendering_manager() const;
		/*! サウンドマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<i_sound_manager> sound_manager() const;
		/*! セーブマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<i_save_manager> save_manager() const;
		/*! デバッグマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<i_debug_manager> debug_manager() const;
		/*! コンフィグマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<i_config_manager> config_manager() const;

	private:
		game();
		game(const game &) = delete;
		game(game &&) = delete;
		~game() override;
		game &operator=(const game &) = delete;
		game &operator=(game &&) = delete;

		class impl;
		std::unique_ptr<impl> impl_;
	};
}
