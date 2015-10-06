#pragma once

#include "UIComponent.h"

namespace planeta_engine {
	namespace ui_objects {
		class TextPanel : public game::UIComponent {
		public:
			const std::string& text()const { return text_; }
			void text(const std::string& t) { text_ = t; }
		private:
			std::string text_;
			virtual void UpdateProc() override;
			virtual void DrawProc(const utility::RectAngle<int>& draw_area) override;
		};
	}
}
