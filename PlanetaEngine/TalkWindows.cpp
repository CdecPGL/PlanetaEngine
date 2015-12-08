#include "TalkWindows.h"

#include "SimplePolygonPanel.h"
#include "TextPanel.h"

namespace planeta_engine {
	namespace ui_objects {

		bool TalkWindow::InitializeProc()
		{
			back_ground_panel_ = AddComponent<ui_components::SimplePolygonPanel>();
			back_ground_panel_->SetVertexes({ math::Vector2Dd(0.0,0.0),math::Vector2Dd(1.0,0.0), math::Vector2Dd(1.0,1.0), math::Vector2Dd(0.0,1.0), });
			back_ground_panel_->fill_color(core::Color::White());
			back_ground_panel_->outline_color(core::Color::Black());
			text_panel_ = back_ground_panel_->AddChild<ui_components::TextPanel>();
			return true;
		}

	}
}