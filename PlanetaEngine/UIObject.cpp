#include "UIObject.h"

namespace planeta_engine {
	namespace game {
		void UIObject::Update()
		{
			UpdateComponents();
			for (auto& c : children_) {
				c.second->Update();
			}
		}

		void UIObject::Draw(const utility::RectAngle<int>& parent_area)
		{
			utility::RectAngle<int> my_area(parent_area.position + position(), size());
			DrawComponents(my_area);
			for (auto& c : children_) {
				c.second->Draw(my_area);
			}
		}

		bool UIObject::KeyInput(ui_object_input_code::type input_code)
		{
			//�t�H�[�J�X���������Ă��Ȃ������珈�����s��Ȃ�
			if (is_focused()) {
				//�����̓��͏������ɍs���A�߂�l���q�ɂ܂킷
				ui_object_input_code::type transfer_child_input_code = KeyInputProc(input_code);
				if (transfer_child_input_code) {
					bool child_input_processed_flag = false;
					for (auto& c : children_) {
						if (c.second->KeyInput(transfer_child_input_code)) { child_input_processed_flag = true; }
					}
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
			for (auto& c : children_) {
				//�q�Ń|�C���e�B���O����Ă�����̂��������画��I��
				if (c.second->PointingCursorPosition(my_relative_cursor_position)) { 
					//�t�H�[�J�X�������ɂ������Ă��Ȃ������瓖�Ă�
					is_focused(true);
					return true;
				}
			}
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

		void UIObject::UpdateComponents()
		{
			for (auto& child : children_) {
				child.second->Update();
			}
		}

		void UIObject::DrawComponents(const utility::RectAngle<int>& draw_area)
		{
			for (auto& child : children_) {
				child.second->Draw(draw_area);
			}
		}

	}
}