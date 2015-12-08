#include "UIObject.h"
#include "UIComponent.h"
#include "DrawManager.h"
#include "SystemLog.h"


namespace planeta_engine {
	namespace game {
		UIObject::UIObject():component_holder_(std::make_unique<utility::ObjectHolderTemplate_WithoutID<UIComponent>>()) {
		}

		void UIObject::Update()
		{
			component_holder_->process();
			component_holder_->do_all([](UIComponent& com) {com.Update(); });
		}

		void UIObject::Draw()
		{
			UIComponent::DrawData my_draw_data;
			my_draw_data.draw_area = rect_angle_;
			my_draw_data.update_info.is_position_updated = is_position_updated_since_last_draw;
			my_draw_data.update_info.is_size_updated = is_size_updated_since_last_draw;
			my_draw_data.update_info.is_edge_fix_updated = false;
			is_position_updated_since_last_draw = false;
			is_size_updated_since_last_draw = false;
			component_holder_->do_all([&my_draw_data](UIComponent& com) {com.Draw(my_draw_data); });
		}

		void UIObject::DebugDraw()
		{
			//�g����\��
			core::DrawManager::instance().DrawUIWire({ rect_angle_.position,math::Vector2Di(rect_angle_.right(),rect_angle_.top()),math::Vector2Di(rect_angle_.right(),rect_angle_.bottom()), math::Vector2Di(rect_angle_.left(),rect_angle_.bottom()),rect_angle_.position }, 1, core::Color::Red());
			component_holder_->do_all([this](UIComponent& com) {com.DebugDraw(rect_angle_); });
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

		bool UIObject::PointingCursorPosition(const math::Vector2Di& parent_relative_cursor_position)
		{
			math::Vector2Di my_relative_cursor_position = parent_relative_cursor_position - position();
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

		bool UIObject::Show()
		{
			if (resister_connection_->Show()) {
				ShowProc();
				return true;
			}
			else {
				return false;
			}
		}

		bool UIObject::Close()
		{
			if (resister_connection_->Close()) {
				CloseProc();
				return true;
			}
			else {
				return false;
			}
		}

		void UIObject::Dispose()
		{
			component_holder_->do_all([](UIComponent& com) {com.Finalize(); });
			FinalizeProc();
			resister_connection_->Dispose();
		}

		bool UIObject::Initialize(std::unique_ptr<UIObjectResisterConnection>&& rc)
		{
			resister_connection_ = std::move(rc);
			if (InitializeProc()==false) {
				debug::SystemLog::instance().LogError("�����������Ɏ��s���܂����B", __FUNCTION__);
				return false;
			}
			if (component_holder_->do_while_return_is_true([](UIComponent& com) {return com.Initialize(); })==false) {
				debug::SystemLog::instance().LogError("�R���|�[�l���g�̏������Ɏ��s���܂����B", __FUNCTION__);
				return false;
			}
			return true;
		}
	}
}