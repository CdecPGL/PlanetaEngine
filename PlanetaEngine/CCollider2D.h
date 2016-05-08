#pragma once
#include "GameObjectSystemComponent.h"
#include "WeakPointer.h"
#include "Vector2D.h"
#include <functional>
#include "Delegate.h"
#include "IColliderWithCollider2D.h"

namespace planeta_engine {
	class CTransform2D;
	class EACollisionWithCollider2D;
	class EACollisionWithGround2D;
	class CCollider2D : public core::GameObjectSystemComponent, public core::IColliderWithCollider2D {
	public:
		virtual ~CCollider2D() = default;
		/*衝突判定を行う*/
		virtual bool DetectCollision(core::IColliderWithCollider2D& collider) = 0;

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
		CCollider2D& relative_position(const Vector2Dd& pos) { position_ = pos; return *this; }
		/*相対回転度を取得*/
		const double relative_rotation_rad()const { return rotation_rad_; }
		/*相対回転度を設定*/
		CCollider2D& relative_rotation_rad(double rota_rad) { rotation_rad_ = rota_rad; return *this; }
		/*地形衝突フラグを取得*/
		const bool collide_with_ground_flag()const { return collide_with_ground_flag_; }
		/*地形衝突フラグを設定*/
		CCollider2D& collide_with_ground_flag(bool flag);
		/*衝突グループを取得*/
		const std::string& collision_group()const { return collision_group_name_; }
		/*衝突グループを設定*/
		CCollider2D& collision_group(const std::string& cg);

		CTransform2D& transform2d() { return *transform2d_; }
		const CTransform2D& transform2d()const { return *transform2d_; }

		/*イベント*/
		/*2Dコライダーとの衝突イベントハンドラ追加*/
		utility::DelegateConnection AddCollidedWithCollider2DEventHandler(utility::DelegateHandlerAdder<EACollisionWithCollider2D> handler_adder);
		/*2D地形との衝突イベントハンドラ追加*/
		utility::DelegateConnection AddCollidedWithGround2DEventHandler(utility::DelegateHandlerAdder<EACollisionWithGround2D> handler_adder);
	protected:
		bool OnInitialized()override;
	private:
		bool OnActivated() override final;
		bool OnInactivated() override final;
		void ResistToCollisionDetectProcess_();
		void RemoveFromCollisionDetectProcess_();

		utility::NonOwingPointer<CTransform2D> transform2d_;

		utility::Delegate<EACollisionWithCollider2D> collided_event_;
		utility::Delegate<EACollisionWithGround2D> collided_with_ground_event_;

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
