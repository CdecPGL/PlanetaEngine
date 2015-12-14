#pragma once

#include "GUIObject.h"

namespace planeta_engine {
	namespace ui_components {
		class SimplePolygonPanel;
		class TextPanel;
	}
	namespace ui_objects {
		class TalkWindow : public game::GUIObject {
		public:

		private:
			bool InitializeProc()override;
			std::shared_ptr<ui_components::SimplePolygonPanel> back_ground_panel_;
			std::shared_ptr<ui_components::TextPanel> text_panel_;
		};
	}
}