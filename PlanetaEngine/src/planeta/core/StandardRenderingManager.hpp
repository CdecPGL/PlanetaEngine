#pragma once

#include "planeta/core/RenderingManager.hpp"

namespace plnt {
	namespace private_{
		class StandardRenderingManager final : public RenderingManager{
		public:
			StandardRenderingManager();
			~StandardRenderingManager() = default;

			bool Initialize() override;
			void Finalize() override;

			//システム関数
			bool Update()override;

			std::shared_ptr<Screen> GetMainScreen()override;
			std::shared_ptr<Screen> GetDebugDrawScreen() override;
			std::shared_ptr<Screen> GetDebugInformationScreen() override;
			std::shared_ptr<Screen> GetGUIScreen() override;
		private:
			std::shared_ptr<Screen> main_screen_; //メインスクリーン
			std::shared_ptr<Screen> gui_screen_; //GUIスクリーン
			std::shared_ptr<Screen> debug_draw_screen_; //デバッグ描画スクリーン
			std::shared_ptr<Screen> debug_information_screen_; //デバッグ情報スクリーン
		};
	}
}
