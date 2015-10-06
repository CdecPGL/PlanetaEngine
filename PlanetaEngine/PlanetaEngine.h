#pragma once
#include <memory>

//ライブラリファイルの指定
#ifdef _DEBUG
#pragma comment(lib,"planetaengine_mtd.lib")
#else
#pragma comment(lib,"planetaengine_mt.lib")
#endif

#include <functional>
#include "Object.h"
#include "SharedPointerInstance.h"

namespace planeta_engine {
	namespace core {
		class IGameSetUp;
		class Game;
		/*ゲームクラスはこのクラスを継承し、初期化を定義する。*/
		class PlanetaEngine : public core::Object,public utility::SharedPointerInstance<PlanetaEngine>{
		public:
			PlanetaEngine();
			~PlanetaEngine();
			/*エンジンの初期化(失敗した場合でも終了処理を呼び出す必要がある)*/
			bool Initialize();
			/*エンジンの終了処理*/
			void Finalize();
			/*エンジンのステータス*/
			enum class Status { Continue, Quit, Error };
			/*エンジンの更新*/
			Status Update();
		protected:
			IGameSetUp& game_setup_access();
		private:
			PlanetaEngine(const PlanetaEngine&) = delete;
			PlanetaEngine(PlanetaEngine&&) = delete;
			PlanetaEngine& operator=(const PlanetaEngine&) = delete;
			PlanetaEngine& operator=(PlanetaEngine&&) = delete;
			bool _is_initialized;
			std::unique_ptr<Game> _game;
			//ゲーム固有の初期化
			virtual bool _InitializeGame() = 0;
			//スタートアップシーン
			virtual std::string _StartUpSceneName()const = 0;
		};
	}
}
