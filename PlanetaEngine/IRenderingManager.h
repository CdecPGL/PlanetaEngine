#pragma once

#include <memory>

namespace planeta {
	namespace private_ {
		class Screen;
	}
	/*! レンダリングマネージャの公開インターフェイス。描画を管理する。*/
	class IRenderingManager {
	public:
		virtual ~IRenderingManager() = 0 {};
		/*! 新しいスクリーンを作成する(現状は一つのスクリーンにのみ対応)*/
		virtual std::shared_ptr<private_::Screen> CreateScreen() = 0;
		/*! スクリーンを破棄する*/
		virtual bool DisposeScreen(const std::shared_ptr<private_::Screen>& screen) = 0;
	};
}