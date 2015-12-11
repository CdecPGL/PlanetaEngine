#include "ColliderComponent.h"
#include "IGameObjectAccessor.h"
#include "CollisionDetectProcess.h"
#include "SystemLog.h"
#include "SceneAccessorForGameObject.h"
#include "TransformComponent.h"
#include "Matrix.h"
#include "GameObjectComponentSpecialSetUpData.h"

namespace planeta_engine {
	namespace components {

		bool ColliderComponent::OnActivated()
		{
			ResistToCollisionDetectProcess_();
			return true;
		}

		bool ColliderComponent::OnInactivated()
		{
			RemoveFromCollisionDetectProcess_();
			return true;
		}

		void ColliderComponent::ResistToCollisionDetectProcess_()
		{
			if (collision_detect_process_) {
				if (collision_group_name_.length() == 0) {
					debug::SystemLog::instance().LogError("衝突グループが設定されていません。", __FUNCTION__);
				} else {
					collision_detect_process_->Resist(std::static_pointer_cast<ColliderComponent>(this_shared()));
				}
			}
			else {
				debug::SystemLog::instance().LogError("衝突判定プロセスが取得できていません。", __FUNCTION__);
			}
		}

		void ColliderComponent::RemoveFromCollisionDetectProcess_()
		{
			if (collision_detect_process_) {
				collision_detect_process_->Remove(this);
			}
			else {
				debug::SystemLog::instance().LogError("衝突判定プロセスが取得できていません。", __FUNCTION__);
			}
		}

		const Vector2D<double> ColliderComponent::GetCollisionCenterPosition() const
		{
			const TransformComponent& transform = game_object().transform();
			Vector2D<double> relation_position = math::RotationalTransformation(transform.global_rotation_rad(), position_); //ゲームオブジェクトからの相対位置
			relation_position.x *= transform.global_scale().x; //横方向拡大を反映
			relation_position.y *= transform.global_scale().y; //縦方向拡大を反映
			return transform.global_position() + relation_position;
		}

		const double ColliderComponent::GetCollisionScale() const
		{
			const TransformComponent& transform = game_object().transform();
			return transform.global_scale().x > transform.global_scale().y ? transform.global_scale().y : transform.global_scale().x;
		}

		const double ColliderComponent::GetCollisionRotationRad() const
		{
			return game_object().transform().global_rotation_rad() + rotation_rad_;
		}

		void ColliderComponent::collision_group(const std::string& cg) {
			if (collision_detect_process_) { //衝突判定プロセスが取得されていたら衝突グループIDを変更する
				if (collision_detect_process_->ChangeCollisionGroup(this, cg)) {
					collision_group_name_ = cg;
				} else {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "衝突グループを", collision_group_name_, "から",cg,"に変更できませんでした。");
				}
			} else {
				collision_group_name_ = cg;
			}
		}

		void ColliderComponent::collide_with_ground_flag(bool flag) {
			if (collision_detect_process_) { //衝突判定プロセスが取得されていたらフラグを変更する
				if (collision_detect_process_->ChangeCollisionWithGroundFlag(this,flag)) {
					collide_with_ground_flag_ = flag;
				} else {
					debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "地形との衝突フラグを", collide_with_ground_flag_ ? "true" : "false", "から", flag ? "true" : "false", "に変更できませんでした。");
				}
			} else {
				collide_with_ground_flag_ = flag;
			}
		}

		bool ColliderComponent::SpecialSetUp(const core::GameObjectComponentSpecialSetUpData& setup_data) {
			collision_detect_process_ = setup_data.collision_detect_process;
			return true;
		}

	}
}