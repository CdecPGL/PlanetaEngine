#pragma once

#include "i_rendering_manager.hpp"
#include "SubSystemManager.hpp"

namespace plnt::private_ {
	class screen;

	/*レンダリングマネージャ*/
	class rendering_manager : public i_rendering_manager, public sub_system_manager {
	public:
		rendering_manager() = default;
		rendering_manager(const rendering_manager &) = delete;
		rendering_manager(rendering_manager &&) = delete;
		~rendering_manager() override = default;
		rendering_manager &operator=(const rendering_manager &) = delete;
		rendering_manager &operator=(rendering_manager &&) = delete;

		/*初期化する*/
		virtual bool initialize() = 0;
		/*終了処理を行う*/
		virtual void finalize() = 0;
		/*更新する*/
		virtual bool update() = 0;
		/*デバッグ描画用スクリーンを取得する*/
		[[nodiscard]] virtual std::shared_ptr<screen> get_debug_draw_screen() = 0;
		/*デバッグ情報描画用スクリーンを取得する*/
		[[nodiscard]] virtual std::shared_ptr<screen> get_debug_information_screen() = 0;
	};
}
