#pragma once

#include <memory>
#include "Object.hpp"
#include "SingletonTemplate.hpp"

namespace plnt {
	class IResourceManager;
	class ILogManager;
	class ISceneManager;
	class IInputManager;
	class IPerformanceManager;
	class IRenderingManager;
	class ISoundManager;
	class ISaveManager;
	class IDebugManager;
	class IConfigManager;
	namespace private_ {
		class ResourceManager;
		class LogManager;
		class SceneManager;
		class InputManager;
		class PerformanceManager;
		class RenderingManager;
		class SoundManager;
		class SaveManager;
		class DebugManager;
		class ConfigManager;
	}
	/*! ゲームのステータス*/
	enum class GameStatus { Continue, Quit, Error };
	/*! PlanetaEngineクラス*/
	class Game final : public util::SingletonTemplate<Game> {
		friend util::SingletonTemplate<Game>;
	public:
		/*! 標準のマネージャを設定する。初期化前に実行しなければならない*/
		void SetStandardManagers();
		/*! エンジンの初期化*/
		bool Initialize()override;
		/*! エンジンの終了処理*/
		void Finalize()override;
		/*! エンジンの更新(初期化が正常に行われていない状態での呼び出しは未定義動作)*/
		GameStatus Update();
		/*! 初期化されているか*/
		bool is_initialized()const;
		/*! リソースマネージャを設定する(初期化前)*/
		void SetResourceManager(const std::shared_ptr<private_::ResourceManager>& mgr);
		/*! ログマネージャを設定する(初期化前)*/
		void SetLogManager(const std::shared_ptr<private_::LogManager>& mgr);
		/*! シーンマネージャを設定する(初期化前)*/
		void SetSceneManager(const std::shared_ptr<private_::SceneManager>& mgr);
		/*! インプットマネージャを設定する(初期化前)*/
		void SetInputManager(const std::shared_ptr<private_::InputManager>& mgr);
		/*! パフォーマンスマネージャを設定する(初期化前)*/
		void SetPerformanceManager(const std::shared_ptr<private_::PerformanceManager>& mgr);
		/*! レンダリングマネージャを設定する(初期化前)*/
		void SetRenderingManager(const std::shared_ptr<private_::RenderingManager>& mgr);
		/*! サウンドマネージャを設定する(初期化前)*/
		void SetSoundManager(const std::shared_ptr<private_::SoundManager>& mgr);
		/*! セーブマネージャを設定する(初期化前)*/
		void SetSaveManager(const std::shared_ptr<private_::SaveManager>& mgr);
		/*! デバッグマネージャを設定する(初期化前)*/
		void SetDebugManager(const std::shared_ptr<private_::DebugManager>& mgr);
		/*! コンフィグマネージャを設定する(初期化前)*/
		void SetConfigManager(const std::shared_ptr<private_::ConfigManager>& mgr);
		/*! リソースマネージャにアクセスする*/
		std::shared_ptr<IResourceManager> resource_manager()const;
		/*! ログマネージャにアクセスする*/
		std::shared_ptr<ILogManager> log_manager()const;
		/*! シーンマネージャにアクセスする*/
		std::shared_ptr<ISceneManager> scene_manager()const;
		/*! インプットマネージャにアクセスする*/
		std::shared_ptr<IInputManager> input_manager()const;
		/*! パフォーマンスマネージャにアクセスする*/
		std::shared_ptr<IPerformanceManager> performance_manager()const;
		/*! レンダリングマネージャにアクセスする*/
		std::shared_ptr<IRenderingManager> rendering_manager()const;
		/*! サウンドマネージャにアクセスする*/
		std::shared_ptr<ISoundManager> sound_manager()const;
		/*! セーブマネージャにアクセスする*/
		std::shared_ptr<ISaveManager> save_manager()const;
		/*! デバッグマネージャにアクセスする*/
		std::shared_ptr<IDebugManager> debug_manager()const;
		/*! コンフィグマネージャにアクセスする*/
		std::shared_ptr<IConfigManager> config_manager()const;
	private:
		Game();
		~Game();

		class Impl_;
		std::unique_ptr<Impl_> impl_;
	};
}
