#include "GUIObject.h"
#include "GUIComponent.h"
#include "DrawManager.h"
#include "SystemLog.h"


namespace planeta_engine {
	namespace game {
		GUIObject::GUIObject():component_holder_(std::make_unique<utility::ObjectHolderTemplate_WithoutID<GUIComponent>>()) {
		}

		void GUIObject::Update()
		{
			component_holder_->process();
			component_holder_->do_all([](GUIComponent& com) {com.Update(); });
		}

		void GUIObject::Draw()
		{
			GUIComponent::DrawData my_draw_data;
			my_draw_data.draw_area = rect_angle_;
			my_draw_data.update_info.is_position_updated = is_position_updated_since_last_draw;
			my_draw_data.update_info.is_size_updated = is_size_updated_since_last_draw;
			my_draw_data.update_info.is_edge_fix_updated = false;
			is_position_updated_since_last_draw = false;
			is_size_updated_since_last_draw = false;
			component_holder_->do_all([&my_draw_data](GUIComponent& com) {com.Draw(my_draw_data); });
		}

		void GUIObject::DebugDraw()
		{
			//�g����\��
			core::DrawManager::instance().DrawUIWire({ rect_angle_.position,Vector2D<int>(rect_angle_.right(),rect_angle_.top()),Vector2D<int>(rect_angle_.right(),rect_angle_.bottom()), Vector2D<int>(rect_angle_.left(),rect_angle_.bottom()),rect_angle_.position }, 1, Color::Red());
			component_holder_->do_all([this](GUIComponent& com) {com.DebugDraw(rect_angle_); });
		}

		bool GUIObject::KeyInput(ui_object_input_code::type input_code)
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

		bool GUIObject::PointingCursorPosition(const Vector2D<int>& parent_relative_cursor_position)
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

		bool GUIObject::Show()
		{
			if (resister_connection_->Show()) {
				ShowProc();
				return true;
			}
			else {
				return false;
			}
		}

		bool GUIObject::Close()
		{
			if (resister_connection_->Close()) {
				CloseProc();
				return true;
			}
			else {
				return false;
			}
		}

		void GUIObject::Dispose()
		{
			component_holder_->do_all([](GUIComponent& com) {com.Finalize(); });
			FinalizeProc();
			resister_connection_->Dispose();
		}

		bool GUIObject::Initialize(std::unique_ptr<GUIObjectResisterConnection>&& rc)
		{
			resister_connection_ = std::move(rc);
			if (InitializeProc()==false) {
				debug::SystemLog::instance().LogError("�����������Ɏ��s���܂����B", __FUNCTION__);
				return false;
			}
			if (component_holder_->do_while_return_is_true([](GUIComponent& com) {return com.Initialize(); })==false) {
				debug::SystemLog::instance().LogError("�R���|�[�l���g�̏������Ɏ��s���܂����B", __FUNCTION__);
				return false;
			}
			return true;
		}
	}
}