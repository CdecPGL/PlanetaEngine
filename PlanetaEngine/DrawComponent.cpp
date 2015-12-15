#include "DrawComponent.h"
#include "IGameObjectAccessor.h"
//#include "GameObjectDrawComponentProcessRegistrator.h"
#include "GameObjectDrawProcessCore.h"
#include "TransformComponent.h"
#include "Matrix.h"
#include "SystemLog.h"
#include "SceneAccessorForGameObject.h"
#include "SceneDataForGameObject.h"

namespace planeta_engine {
	namespace components {

		DrawComponent::DrawComponent() :draw_priority_(0), rotation_rad_(0.0), scale_(1.0, 1.0)
		{

		}

		void DrawComponent::draw_priority(int priority)
		{
			draw_priority_ = priority;
			//ゲームオブジェクトがアクティブなら優先度更新
			UpdatePriority_();
		}

		Vector2D<double> DrawComponent::GetDrawCenterPosition() const
		{
			const TransformComponent& transform = game_object().transform();
			Vector2D<double> relation_position = math::RotationalTransformation(transform.global_rotation_rad(), position_); //ゲームオブジェクトからの相対位置
			relation_position.x *= transform.global_scale().x; //横方向拡大を反映
			relation_position.y *= transform.global_scale().y; //縦方向拡大を反映
			return transform.global_position() + relation_position;
		}

		double DrawComponent::GetDrawRotationRed() const
		{
			return game_object().transform().global_rotation_rad() + rotation_rad_;
		}

		Vector2D<double> DrawComponent::GetDrawScale() const
		{
			return Vector2D<double>(game_object().transform().global_scale().x * scale_.x, game_object().transform().global_scale().y * scale_.y);
		}

		void DrawComponent::RegisterToProcess_()
		{
			if (draw_component_registrator_) {
				draw_component_registrator_->Register(std::static_pointer_cast<DrawComponent>(this_shared()), draw_priority_);
			}
			else {
				debug::SystemLog::instance().LogWarning("ゲームオブジェクト描画プロセスが取得できていません。", "DrawComponent::_ResistToDrawProcess");
			}
		}

		void DrawComponent::RemoveFromProcess_()
		{
			if (draw_component_registrator_) {
				draw_component_registrator_->Remove(std::static_pointer_cast<DrawComponent>(this_shared()));
			}
			else {
				debug::SystemLog::instance().LogWarning("ゲームオブジェクト描画プロセスが取得できていません。", "DrawComponent::_RemoveFromDrawProcess");
			}
		}

		void DrawComponent::UpdatePriority_()
		{
			if (is_active()) {
				draw_component_registrator_->ChangePriority(std::static_pointer_cast<DrawComponent>(this_shared()), draw_priority_);
			}
		}

		bool DrawComponent::OnActivated()
		{
			RegisterToProcess_();
			return true;
		}

		bool DrawComponent::OnInactivated()
		{
			RemoveFromProcess_();
			return true;
		}

		bool DrawComponent::SpecialSetUp(const core::SceneDataForGameObject& setup_data) {
//			draw_component_registrator_ = setup_data.draw_component_process_registrator;
			draw_component_registrator_ = setup_data.draw_component_process_registrator;
			return true;
		}

	}
}