#pragma once

#include "rendering_manager.hpp"

namespace plnt {
	namespace private_ {
		class StandardRenderingManager final : public rendering_manager {
		public:
			StandardRenderingManager();
			~StandardRenderingManager() = default;

			bool initialize() override;
			void finalize() override;

			//システム関数
			bool update() override;

			std::shared_ptr<screen> get_main_screen() override;
			std::shared_ptr<screen> get_debug_draw_screen() override;
			std::shared_ptr<screen> get_debug_information_screen() override;
			std::shared_ptr<screen> get_gui_screen() override;

		private:
			std::shared_ptr<screen> main_screen_; //メインスクリーン
			std::shared_ptr<screen> gui_screen_; //GUIスクリーン
			std::shared_ptr<screen> debug_draw_screen_; //デバッグ描画スクリーン
			std::shared_ptr<screen> debug_information_screen_; //デバッグ情報スクリーン
		};
	}
}
