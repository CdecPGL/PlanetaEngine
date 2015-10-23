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
			//位置、サイズの計算
			my_draw_data.draw_area.position = parent_draw_data.draw_area.position + position();
			my_draw_data.draw_area.size = size();
			//更新フラグの判定
			my_draw_data.is_position_updated = parent_draw_data.is_position_updated | is_position_updated_since_last_draw;
			my_draw_data.is_size_updated = parent_draw_data.is_size_updated | is_size_updated_since_last_draw;
			is_position_updated_since_last_draw = false;
			is_size_updated_since_last_draw = false;
			//更新フラグに沿って関数呼び出し
			if (my_draw_data.is_position_updated) { PositionUpdatedProc(my_draw_data.draw_area.position); }
			if (my_draw_data.is_size_updated) { SizeUpdatedProc(my_draw_data.draw_area.size); }
			//自分と子の描画処理
			DrawProc(my_draw_data.draw_area);
			DrawChildren(my_draw_data);
		}

		void UIComponent::Update()
		{
			UpdateProc();
			children_holder_->process(); //子オブジェクト更新の前に登録、登録解除処理を行っておく
			UpdateChildren();
		}

	}
}