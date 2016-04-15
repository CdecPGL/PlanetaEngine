#pragma once
#include "GameObjectSpecialComponent.h"
#include "WeakPointer.h"
#include "Vector2D.h"
#include <functional>
#include "WeakPointerDelegate.h"
#include "IColliderWithCollider.h"

namespace planeta_engine {
	namespace event_arguments {
		class CollisionEventArgument;
		class CollisionWithGroundEventArgument;
	}
	namespace system_processes {
		class CollisionDetectProcess;
	}
	namespace components {
		class ColliderComponent : public core::GameObjectSpecialComponent , public core::IColliderWithCollider{
		public:
			virtual ~ColliderComponent() = default;
			/*衝突判定を行う*/
			virtual bool DetectCollision(core::IColliderWithCollider& collider) = 0;

			/*グローバル中心位置取得*/
			const Vector2Dd GetCollisionGlobalCenterPosition()const;
			/*拡大度取得(トランスフォームの拡大度xyで小さい方の要素が適用される)*/
			const double GetCollisionScale()const;
			/*グローバル回転度取得*/
			const double GetCollisionGlobalRotationRad()const;

			/*アクセサ*/
			/*相対位置を取得*/
			const Vector2Dd& relative_position()const { return position_; }
			/*相対位置を設定*/
			ColliderComponent& relative_position(const Vector2Dd& pos) { position_ = pos; return *this; }
			/*相対回転度を取得*/
			const double relative_rotation_rad()const { return rotation_rad_; }
			/*相対回転度を設定*/
			ColliderComponent& relative_rotation_rad(double rota_rad) { rotation_rad_ = rota_rad; return *this; }
			/*地形衝突フラグを取得*/
			const bool collide_with_ground_flag()const { return collide_with_ground_flag_; }
			/*地形衝突フラグを設定*/
			ColliderComponent& collide_with_ground_flag(bool flag);
			/*衝突グループを取得*/
			const std::string& collision_group()const { return collision_group_name_; }
			/*衝突グループを設定*/
			ColliderComponent& collision_group(const std::string& cg);

			/*イベント*/
			/*物体と衝突した*/
			utility::WeakPointerDelegate<event_arguments::CollisionEventArgument> collided;
			/*地形と衝突した*/
			utility::WeakPointerDelegate<event_arguments::CollisionWithGroundEventArgument> collided_with_ground;

		private:
			/*特殊セットアップ*/
			bool SpecialSetUp(const core::SceneDataForGameObject& setup_data)override final;

			bool is_no_update() const override final { return true; }
			bool OnActivated() override final;
			bool OnInactivated() override final;
			utility::WeakPointer<system_processes::CollisionDetectProcess> collision_detect_process_;
			void ResistToCollisionDetectProcess_();
			void RemoveFromCollisionDetectProcess_();

			/*位置*/
			Vector2Dd position_;
			/*回転度*/
			double rotation_rad_ = 0.0;
			/*地形衝突フラグ*/
			bool collide_with_ground_flag_ = false;
			/*衝突グループ名*/
			std::string collision_group_name_;
		};
	}
}
