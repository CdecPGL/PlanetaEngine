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

		void UIObject::Draw()
		{
			UIComponent::DrawData my_draw_data;
			my_draw_data.draw_area = rect_angle_;
			my_draw_data.is_position_updated = is_position_updated_since_last_draw;
			my_draw_data.is_size_updated = is_size_updated_since_last_draw;
			is_position_updated_since_last_draw = false;
			is_size_updated_since_last_draw = false;
			component_holder_->do_all([&my_draw_data](UIComponent& com) {com.Draw(my_draw_data); });
		}

		bool UIObject::KeyInput(ui_object_input_code::type input_code)
		{
			//�t�H�[�J�X���������Ă��Ȃ������珈�����s��Ȃ�
			if (is_focused()) {
				//�����̓��͏������ɍs���A�߂�l���q�ɂ܂킷
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
			//�q����
			//for (auto& c : children_) {
			//	//�q�Ń|�C���e�B���O����Ă�����̂��������画��I��
			//	if (c.second->PointingCursorPosition(my_relative_cursor_position)) { 
			//		//�t�H�[�J�X�������ɂ������Ă��Ȃ������瓖�Ă�
			//		is_focused(true);
			//		return true;
			//	}
			//}
			//����
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