#include "UIObject.h"
#include "UIComponent.h"

namespace planeta_engine {
	namespace game {
		UIObject::UIObject():component_holder_(std::make_unique<utility::ObjectHolderTemplate_WithoutID<UIComponent>>()) {
		}

		void UIObject::Update()
		{
			component_holder_->do_all([](UIComponent& com) {com.Update(); });
		}

		void UIObject::Draw(const utility::RectAngle<int>& parent_area)
		{
			utility::RectAngle<int> my_area(parent_area.position + position(), size());
			component_holder_->do_all([&my_area](UIComponent& com) {com.Draw(my_area); });
		}

		bool UIObject::KeyInput(ui_object_input_code::type input_code)
		{
			//フォーカスが当たっていなかったら処理を行わない
			if (is_focused()) {
				//自分の入力処理を先に行い、戻り値を子にまわす
				ui_object_input_code::type transfer_child_input_code = KeyInputProc(input_code);
				if (transfer_child_input_code) {
					bool child_input_processed_flag = false;
					/*for (auto& c : children_) {
						if (c.second->KeyInput(transfer_child_input_code)) { child_input_processed_flag = true; }
					}*/
					return child_input_processed_flag;
				}
				else {
					return false;
				}
			}
			else { return false; }
		}

		bool UIObject::PointingCursorPosition(const Vector2D<int>& parent_relative_cursor_position)
		{
			Vector2D<int> my_relative_cursor_position = parent_relative_cursor_position - position();
			//子から
			//for (auto& c : children_) {
			//	//子でポインティングされているものがあったら判定終了
			//	if (c.second->PointingCursorPosition(my_relative_cursor_position)) { 
			//		//フォーカスが自分にあたっていなかったら当てる
			//		is_focused(true);
			//		return true;
			//	}
			//}
			//自分
			if (DetectCursorPointedProc(my_relative_cursor_position)) {
				is_focused(true);
				return true;
			}
			else {
				is_focused(false);
				return false;
			}
		}
	}
}