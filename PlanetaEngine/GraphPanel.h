#pragma once

#include <memory>
#include "UIComponent.h"

namespace planeta_engine {
	namespace resources {
		class GraphResource;
	}
	namespace ui_objects {
		class GraphPanel : public game::UIComponent {
		public:
			bool SetGraphResource(const std::string& resource_id);
			/*�A�N�Z�T*/
			const utility::RectAngle<int>& draw_area()const { return draw_area_; }
			void draw_area(const utility::RectAngle<int>& rect) { draw_area_ = rect;}
			const bool reverse()const { return reverse_; }
			void reverse(bool rev) { reverse_ = rev; }
		private:
			virtual void UpdateProc() override;
			virtual void DrawProc(const utility::RectAngle<int>& draw_area) override;
			std::shared_ptr<resources::GraphResource> graph_resource_;
			/*�摜��ŕ\������͈�*/
			utility::RectAngle<int> draw_area_;
			/*���]�`��t���O*/
			bool reverse_ = false;
		};
	}
}
