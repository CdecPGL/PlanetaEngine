#pragma once

#include <memory>

namespace plnt {
	namespace private_ {
		class screen;
	}

	/*! レンダリングマネージャの公開インターフェイス。描画を管理する。*/
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_rendering_manager {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_rendering_manager() = 0 { }
		/*! メインスクリーンを取得する*/
		[[nodiscard]] virtual std::shared_ptr<private_::screen> get_main_screen() = 0;
		/*! GUIクリーンを取得する*/
		[[nodiscard]] virtual std::shared_ptr<private_::screen> get_gui_screen() = 0;
	};
}
