#include "UIComponent.h"

namespace planeta_engine {
	namespace game {

		UIComponent::UIComponent():children_holder_(std::make_unique<utility::ObjectHolderTemplate_WithoutID<UIComponent>>()){}

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
			DrawData my_draw_data;
			//�ʒu�A�T�C�Y�̌v�Z
			my_draw_data.draw_area.position = parent_draw_data.draw_area.position + position();
			my_draw_data.draw_area.size = size();
			//�X�V�t���O�̔���
			my_draw_data.is_position_updated = parent_draw_data.is_position_updated | is_position_updated_since_last_draw;
			my_draw_data.is_size_updated = parent_draw_data.is_size_updated | is_size_updated_since_last_draw;
			is_position_updated_since_last_draw = false;
			is_size_updated_since_last_draw = false;
			//�X�V�t���O�ɉ����Ċ֐��Ăяo��
			if (my_draw_data.is_position_updated) { PositionUpdatedProc(my_draw_data.draw_area.position); }
			if (my_draw_data.is_size_updated) { SizeUpdatedProc(my_draw_data.draw_area.size); }
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

	}
}