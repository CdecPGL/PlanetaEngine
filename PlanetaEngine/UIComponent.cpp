#include "UIComponent.h"
#include "DrawManager.h"
#include "SystemLog.h"
#include <cassert>

namespace planeta_engine {
	namespace game {

		UIComponent::UIComponent() :children_holder_(std::make_unique<utility::ObjectHolderTemplate_WithoutID<UIComponent>>()) {}

		void UIComponent::UpdateChildren()
		{
			children_holder_->do_all([](UIComponent& com) {com.Update(); });
		}

		void UIComponent::DrawChildren(const DrawData& my_draw_data)
		{
			children_holder_->do_all([&my_draw_data](UIComponent& com) {com.Draw(my_draw_data); });
		}

		void UIComponent::Draw(const DrawData& parent_draw_data)
		{
			UpdateInfo my_update_info;
			//�X�V�t���O�̔���
			my_update_info.is_position_updated = is_position_updated_since_last_draw;
			my_update_info.is_size_updated = is_size_updated_since_last_draw;
			my_update_info, is_edge_fix_updated_since_last_draw = is_edge_fix_updated_since_last_draw;
			is_position_updated_since_last_draw = false;
			is_size_updated_since_last_draw = false;
			is_edge_fix_updated_since_last_draw = false;
			//�`��ʒu���ƍX�V���̌v�Z
			CalculateDrawRectAndUpdateInfo(parent_draw_data, &my_update_info);
			//�X�V�t���O�ɉ����Ċ֐��Ăяo��
			if (my_update_info.is_position_updated) { PositionUpdatedProc(relative_draw_area_buffer_.position); }
			if (my_update_info.is_size_updated) { SizeUpdatedProc(relative_draw_area_buffer_.size); }
			//�����̕`����(�ʒu�͑��Έʒu�Ɛe�̈ʒu�̘a�A�T�C�Y�͎����̃T�C�Y)
			DrawData my_draw_data = { utility::RectAngle<int>(parent_draw_data.draw_area.position + relative_draw_area_buffer_.position,relative_draw_area_buffer_.size),my_update_info };
			//�����Ǝq�̕`�揈��
			DrawProc(my_draw_data.draw_area);
			DrawChildren(my_draw_data);
		}

		void UIComponent::Update()
		{
			UpdateProc();
			children_holder_->process(); //�q�I�u�W�F�N�g�X�V�̑O�ɓo�^�A�o�^�����������s���Ă���
			UpdateChildren();
		}

		void UIComponent::FixEdge(edge e, int padding)
		{
			edge_fix_bit_set_.set(static_cast<unsigned int>(e), true);
			edge_fix_padding_[static_cast<unsigned int>(e)] = padding;
			is_edge_fix_updated_since_last_draw = true;
		}

		void UIComponent::FixAllEdge(int left_padding, int right_padding, int top_padding, int bottom_padding)
		{
			edge_fix_bit_set_.set();
			edge_fix_padding_[static_cast<unsigned int>(edge::Left)] = left_padding;
			edge_fix_padding_[static_cast<unsigned int>(edge::Right)] = right_padding;
			edge_fix_padding_[static_cast<unsigned int>(edge::Top)] = top_padding;
			edge_fix_padding_[static_cast<unsigned int>(edge::Bottom)] = bottom_padding;
			is_edge_fix_updated_since_last_draw = true;
		}


		void UIComponent::UnfixEdge(edge e)
		{
			edge_fix_bit_set_.set(static_cast<unsigned int>(e), false);
			is_edge_fix_updated_since_last_draw = true;
		}

		void UIComponent::CalculateDrawRectAndUpdateInfo(const DrawData& parent_draw_data, UpdateInfo* my_update_info)
		{
			//�����̈ʒu���X�V����Ă�����X�V����
			if (my_update_info->is_position_updated) {
				relative_draw_area_buffer_.position = rect_angle_.position;
			}
			//�����̃T�C�Y���X�V����Ă�����X�V����
			if (my_update_info->is_size_updated) {
				relative_draw_area_buffer_.size = rect_angle_.size;
			}
			//�����̕ӌŒ�ʒu�T�C�Y���X�V���ꂽ���A�e�̈ʒu���X�V����Ă�����A���̌v�Z���s��(����͂���ɍœK���ł�����)
			if (my_update_info->is_edge_fix_updated |my_update_info->is_position_updated | my_update_info->is_size_updated | parent_draw_data.update_info.is_size_updated) {
				static constexpr unsigned int er = 1u << static_cast<unsigned int>(edge::Right);
				static constexpr unsigned int el = 1u << static_cast<unsigned int>(edge::Left);
				static constexpr unsigned int et = 1u << static_cast<unsigned int>(edge::Top);
				static constexpr unsigned int eb = 1u << static_cast<unsigned int>(edge::Bottom);

				const unsigned int e = edge_fix_bit_set_.to_ulong();

				if ((e & er) != 0 && (e & el) != 0) { //���E
					relative_draw_area_buffer_.position.x = edge_fix_padding_[static_cast<unsigned int>(edge::Left)];
					relative_draw_area_buffer_.size.x = parent_draw_data.draw_area.size.x - edge_fix_padding_[static_cast<unsigned int>(edge::Left)] - edge_fix_padding_[static_cast<unsigned int>(edge::Right)];
					if (relative_draw_area_buffer_.size.x < 0) { relative_draw_area_buffer_.size.x = 0; }
					my_update_info->is_position_updated = true;
					my_update_info->is_size_updated = true;
				}
				else if (e & er) { //�E
					relative_draw_area_buffer_.position.x = parent_draw_data.draw_area.size.x - edge_fix_padding_[static_cast<unsigned int>(edge::Right)] - rect_angle_.size.x;
					my_update_info->is_position_updated = true;
				}
				else if (e & el) { //��
					relative_draw_area_buffer_.position.x = edge_fix_padding_[static_cast<unsigned int>(edge::Left)];
					my_update_info->is_position_updated = true;
				}
				if ((e & et) != 0 && (e & eb) != 0) { //�㉺
					relative_draw_area_buffer_.position.y = edge_fix_padding_[static_cast<unsigned int>(edge::Top)];
					relative_draw_area_buffer_.size.y = parent_draw_data.draw_area.size.y - edge_fix_padding_[static_cast<unsigned int>(edge::Top)] - edge_fix_padding_[static_cast<unsigned int>(edge::Bottom)];
					if (relative_draw_area_buffer_.size.y < 0) { relative_draw_area_buffer_.size.y = 0; }
					my_update_info->is_position_updated = true;
					my_update_info->is_size_updated = true;
				}
				else if (e & et) { //��
					relative_draw_area_buffer_.position.y = parent_draw_data.draw_area.size.y - edge_fix_padding_[static_cast<unsigned int>(edge::Bottom)] - rect_angle_.size.y;
					my_update_info->is_position_updated = true;
				}
				else if (e & eb) { //��
					relative_draw_area_buffer_.position.y = edge_fix_padding_[static_cast<unsigned int>(edge::Top)];
					my_update_info->is_position_updated = true;
				}
			}
			my_update_info->is_position_updated |= parent_draw_data.update_info.is_position_updated;
		}

		void UIComponent::DebugDraw(const utility::RectAngle<int>& parent_draw_area)
		{
			//�g����\��
			utility::RectAngle<int> my_draw_area(parent_draw_area.position + relative_draw_area_buffer_.position,relative_draw_area_buffer_.size);
			core::DrawManager::instance().DrawUIWire({ my_draw_area.position,Vector2D<int>(my_draw_area.right(),my_draw_area.top()),Vector2D<int>(my_draw_area.right(),my_draw_area.bottom()), Vector2D<int>(my_draw_area.left(),my_draw_area.bottom()),my_draw_area.position }, 1, Color(255, 150, 0));
			children_holder_->do_all([&my_draw_area](UIComponent& com) {com.DebugDraw(my_draw_area); });
		}

		bool UIComponent::Initialize()
		{
			if (InitializeProc() == false) {
				debug::SystemLog::instance().LogError("�����������Ɏ��s���܂����B", __FUNCTION__);
				return false;
			}
			if (children_holder_->do_while_return_is_true([](UIComponent& com) {return com.Initialize(); }) == false) {
				debug::SystemLog::instance().LogError("�q�R���|�[�l���g�̏������Ɏ��s���܂����B", __FUNCTION__);
				return false;
			}
			return true;
		}

		void UIComponent::Finalize()
		{
			children_holder_->do_all([](UIComponent& com) {com.Finalize(); });
			FinalizeProc();
		}

	}
}