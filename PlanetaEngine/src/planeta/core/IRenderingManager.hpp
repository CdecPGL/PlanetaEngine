#pragma once

#include <memory>

namespace plnt {
	namespace private_ {
		class Screen;
	}

	/*! レンダリングマネージャの公開インターフェイス。描画を管理する。*/
	class IRenderingManager {
	public:
		virtual ~IRenderingManager() = 0 { };
		/*! メインスクリーンを取得する*/
		virtual std::shared_ptr<private_::Screen> GetMainScreen() = 0;
		/*! GUIクリーンを取得する*/
		virtual std::shared_ptr<private_::Screen> GetGUIScreen() = 0;
	};
}
