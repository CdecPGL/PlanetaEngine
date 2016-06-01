#pragma once


//ライブラリファイルの指定
#ifdef _DEBUG
#pragma comment(lib,"planetaengine_mtd.lib")
#else
#pragma comment(lib,"planetaengine_mt.lib")
#endif

#include <memory>
#include "Object.h"
#include "SingletonTemplate.h"

namespace planeta_engine {
	/*ゲームのステータス*/
	enum class GameStatus { Continue, Quit, Error };
	/*ゲームクラスはこのクラスを継承し、初期化を定義する。*/
	class PlanetaEngine final : public util::SingletonTemplate<PlanetaEngine> {
		friend util::SingletonTemplate<PlanetaEngine>;
	public:
		/*エンジンの初期化*/
		bool Initialize()override;
		/*エンジンの終了処理*/
		void Finalize()override;
		/*エンジンの更新(初期化が正常に行われていない状態での呼び出しは未定義動作)*/
		GameStatus Update();
	private:
		PlanetaEngine();
		~PlanetaEngine();

		class Impl_;
		std::unique_ptr<Impl_> impl_;
	};
}
