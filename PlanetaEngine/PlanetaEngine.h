#pragma once


//ライブラリファイルの指定
#ifdef _DEBUG
#pragma comment(lib,"planetaengine_mtd.lib")
#else
#pragma comment(lib,"planetaengine_mt.lib")
#endif

#include <memory>
#include "Object.h"
#include "StaticSingletonTemplate.h"

namespace planeta_engine {
	namespace core {
		class Game;
	}
	/*ゲームクラスはこのクラスを継承し、初期化を定義する。*/
	class PlanetaEngine final : public utility::StaticSingletonTemplate<PlanetaEngine> {
		friend utility::StaticSingletonTemplate<PlanetaEngine>;
	public:
		/*ゲームの作成(初期化前に行う)*/
		template<class GameType>
		bool CreateGameInstance() {
			game_ = std::make_unique<GameType>();
			return game_ != nullptr;
		}
		/*エンジンの初期化*/
		bool Initialize()override;
		/*エンジンの終了処理*/
		void Finalize()override;
		/*エンジンのステータス*/
		enum class Status { Continue, Quit, Error };
		/*エンジンの更新(初期化が正常に行われていない状態での呼び出しは未定義動作)*/
		Status Update();
	private:
		PlanetaEngine();
		~PlanetaEngine();
		PlanetaEngine(const PlanetaEngine&) = delete;
		PlanetaEngine(PlanetaEngine&&) = delete;
		PlanetaEngine& operator=(const PlanetaEngine&) = delete;
		PlanetaEngine& operator=(PlanetaEngine&&) = delete;
		bool is_initialized_;
		std::unique_ptr<core::Game> game_;

		bool InitializeEngine();
		void FinalzieEngine();

		bool InitializeDebugSystem();
		void FinalizeDebugSystem();
	};
}
