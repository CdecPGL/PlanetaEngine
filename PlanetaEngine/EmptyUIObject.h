#pragma once

#include "UIObject.h"

namespace planeta_engine {
	namespace ui_objects {
		class EmptyUIObject final: public game::UIObject {
		public:
		private:
			void UpdateComponents() override {}
			void DrawComponents(const utility::RectAngle<int>& draw_area) override {}
		};
	}
}
