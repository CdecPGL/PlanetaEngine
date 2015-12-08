#pragma once
#include "Component.h"
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
		class ColliderComponent : public game::Component , public core::IColliderWithCollider{
		public:
			virtual ~ColliderComponent() = default;
			/*衝突判定を行う*/
			virtual bool DetectCollision(core::IColliderWithCollider& collider) = 0;

			/*中心位置取得*/
			const math::Vector2Dd GetCollisionCenterPosition()const;
			/*拡大度取得(トランスフォームの拡大度xyで小さい方の要素が適用される)*/
			const double GetCollisionScale()const;
			/*回転度取得*/
			const double GetCollisionRotationRad()const;

			/*アクセサ*/
			/*位置を取得*/
			const math::Vector2Dd& position()const { return position_; }
			/*位置を設定*/
			void position(const math::Vector2Dd& pos) { position_ = pos; }
			/*回転度を取得*/
			const double rotation_rad()const { return rotation_rad_; }
			/*回転度を設定*/
			void rotation_rad(double rota_rad) { rotation_rad_ = rota_rad; }
			/*地形衝突フラグを取得*/
			const bool collide_with_ground_flag()const { return collide_with_ground_flag_; }
			/*地形衝突フラグを設定*/
			void collide_with_ground_flag(bool flag);
			/*衝突グループを取得*/
			const std::string& collision_group()const { return collision_group_name_; }
			/*衝突グループを設定*/
			void collision_group(const std::string& cg);

			/*イベント*/
			/*物体と衝突した*/
			utility::WeakPointerDelegate<event_arguments::CollisionEventArgument> collided;
			/*地形と衝突した*/
			utility::WeakPointerDelegate<event_arguments::CollisionWithGroundEventArgument> collided_with_ground;

		protected:
			virtual bool Initialize_() override;
			virtual void Finalize_()noexcept override;
		private:
			bool NoUpdate_() const override final { return true; }
			bool Activated_() override final;
			bool InActivated_() override final;
			utility::WeakPointer<system_processes::CollisionDetectProcess> _collision_detect_process;
			void _ResistToCollisionDetectProcess();
			void _RemoveFromCollisionDetectProcess();

			/*位置*/
			math::Vector2Dd position_;
			/*回転度*/
			double rotation_rad_ = 0.0;
			/*地形衝突フラグ*/
			bool collide_with_ground_flag_ = false;
			/*衝突グループ名*/
			std::string collision_group_name_;
		};
	}
}
