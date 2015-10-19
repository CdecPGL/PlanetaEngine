#include "UIComponent.h"

namespace planeta_engine {
	namespace game {

		UIComponent::UIComponent():children_holder_(std::make_unique<utility::ObjectHolderTemplate_WithoutID<UIComponent>>()){}

		void UIComponent::Draw(const utility::RectAngle<int>& holder_draw_area)
		{
			utility::RectAngle<int> my_draw_area(holder_draw_area.position + position(), size());
			////â°
			//if (holder_draw_area.size.x < size().x) { my_draw_area.position.x = holder_draw_area.left(); my_draw_area.size.x = holder_draw_area.size.x; }
			//else if (rect().left() < 0) { my_draw_area.position.x = holder_draw_area.left(); }
			//else if (rect().right() >= holder_draw_area.size.x) { my_draw_area.position.x = holder_draw_area.right() - size().x; }
			////èc
			//if (holder_draw_area.size.y < size().y) { my_draw_area.position.y = holder_draw_area.top(); my_draw_area.size.y = holder_draw_area.size.y; }
			//else if (rect().top() < 0) { my_draw_area.position.y = holder_draw_area.top(); }
			//else if (rect().bottom() >= holder_draw_area.size.y) { my_draw_area.position.y = holder_draw_area.bottom() - size().y; }

			DrawProc(my_draw_area);
			DrawChildren();
		}

		void UIComponent::UpdateChildren()
		{
			children_holder_->do_all([](UIComponent& com) {com.Update(); });
		}

		void UIComponent::DrawChildren(const utility::RectAngle<int>& my_draw_area)
		{
			children_holder_->do_all([&my_draw_area](UIComponent& com) {com.Draw(my_draw_area); });
		}

	}
}