#pragma once

#include <memory>

#include "SingletonTemplate.hpp"

namespace plnt {
	class IResourceManager;
	class i_log_manager;
	class ISceneManager;
	class i_input_manager;
	class IPerformanceManager;
	class IRenderingManager;
	class ISoundManager;
	class ISaveManager;
	class i_debug_manager;
	class i_config_manager;

	namespace private_ {
		class ResourceManager;
		class LogManager;
		class SceneManager;
		class InputManager;
		class PerformanceManager;
		class RenderingManager;
		class SoundManager;
		class SaveManager;
		class debug_manager;
		class config_manager;
	}

	/*! ゲームのステータス*/
	enum class game_status { play, quit, error };

	/*! PlanetaEngineクラス*/
	class game final : public util::SingletonTemplate<game> {
		friend SingletonTemplate;

	public:
		/*! 標準のマネージャを設定する。初期化前に実行しなければならない*/
		void set_standard_managers() const;
		/*! エンジンの初期化*/
		bool Initialize() override;
		/*! エンジンの終了処理*/
		void Finalize() override;
		/*! エンジンの更新(初期化が正常に行われていない状態での呼び出しは未定義動作)*/
		[[nodiscard]] game_status update() const;
		/*! 初期化されているか*/
		[[nodiscard]] bool is_initialized() const;
		/*! リソースマネージャを設定する(初期化前)*/
		void set_resource_manager(const std::shared_ptr<private_::ResourceManager> &mgr) const;
		/*! ログマネージャを設定する(初期化前)*/
		auto set_log_manager(const std::shared_ptr<private_::LogManager> &mgr) const -> void;
		/*! シーンマネージャを設定する(初期化前)*/
		void set_scene_manager(const std::shared_ptr<private_::SceneManager> &mgr) const;
		/*! インプットマネージャを設定する(初期化前)*/
		void set_input_manager(const std::shared_ptr<private_::InputManager> &mgr) const;
		/*! パフォーマンスマネージャを設定する(初期化前)*/
		void set_performance_manager(const std::shared_ptr<private_::PerformanceManager> &mgr) const;
		/*! レンダリングマネージャを設定する(初期化前)*/
		void set_rendering_manager(const std::shared_ptr<private_::RenderingManager> &mgr) const;
		/*! サウンドマネージャを設定する(初期化前)*/
		void set_sound_manager(const std::shared_ptr<private_::SoundManager> &mgr) const;
		/*! セーブマネージャを設定する(初期化前)*/
		void set_save_manager(const std::shared_ptr<private_::SaveManager> &mgr) const;
		/*! デバッグマネージャを設定する(初期化前)*/
		void set_debug_manager(const std::shared_ptr<private_::debug_manager> &mgr) const;
		/*! コンフィグマネージャを設定する(初期化前)*/
		void set_config_manager(const std::shared_ptr<private_::config_manager> &mgr) const;
		/*! リソースマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<IResourceManager> resource_manager() const;
		/*! ログマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<i_log_manager> log_manager() const;
		/*! シーンマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<ISceneManager> scene_manager() const;
		/*! インプットマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<i_input_manager> input_manager() const;
		/*! パフォーマンスマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<IPerformanceManager> performance_manager() const;
		/*! レンダリングマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<IRenderingManager> rendering_manager() const;
		/*! サウンドマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<ISoundManager> sound_manager() const;
		/*! セーブマネージャにアクセスする*/
		[[nodiscard]] std::shared_ptr<ISaveManager> save_manager() const;
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
