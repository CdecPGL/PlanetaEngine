#pragma once

#include <memory>
#include "Object.h"
#include "SingletonTemplate.h"

namespace planeta {
	/*ゲームのステータス*/
	enum class GameStatus { Continue, Quit, Error };
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
