#pragma once

#include <memory>
#include "Object.h"
#include "SingletonTemplate.h"

namespace planeta {
	class IResourceManager;
	class ILogManager;
	namespace private_ {
		class ResourceManager;
		class LogManager;
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
		/*! リソースマネージャにアクセスする*/
		std::shared_ptr<IResourceManager> resource_manager()const;
		/*! ログマネージャにアクセスする*/
		std::shared_ptr<ILogManager> log_manager()const;
	private:
		Game();
		~Game();

		class Impl_;
		std::unique_ptr<Impl_> impl_;
	};
}
