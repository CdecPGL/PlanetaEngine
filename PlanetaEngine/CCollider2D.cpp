#include "CCollider2D.h"
#include "IGameObjectForComponent.h"
#include "CollisionWorld.h"
#include "SystemLog.h"
#include "SceneData.h"
#include "CTransform2D.h"
#include "Matrix2_2.h"
#include "Collider2DData.h"

namespace planeta {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CCollider2D) {
		registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(CCollider2D, relative_position)
		.PE_REFLECTABLE_CLASS_PROPERTY(CCollider2D, relative_rotation_rad)
		.PE_REFLECTABLE_CLASS_PROPERTY(CCollider2D, collide_with_ground_flag)
		.PE_REFLECTABLE_CLASS_PROPERTY(CCollider2D, collision_group)
		.ShallowCopyTarget(&CCollider2D::position_)
		.ShallowCopyTarget(&CCollider2D::rotation_rad_)
		.ShallowCopyTarget(&CCollider2D::collide_with_ground_flag_)
		.ShallowCopyTarget(&CCollider2D::collision_group_name_);
	}

	bool CCollider2D::OnActivated() {
		ResistToCollisionDetectProcess_();
		return true;
	}

	bool CCollider2D::OnInactivated() {
		RemoveFromCollisionDetectProcess_();
		return true;
	}

	void CCollider2D::ResistToCollisionDetectProcess_() {
		if (collision_group_name_.length() == 0) {
			PE_LOG_ERROR("衝突グループが設定されていません。");
		} else {
			private_::Collider2DData col_dat{ *this,game_object(),*transform2d_
				,[&eve = collided_event_](const EACollisionWithCollider2D& arg) {eve(arg); }
				,[&eve = collided_with_ground_event_](const EACollisionWithGround2D& arg) {eve(arg); }
			};
			scene_data_ref().collision_world.Resist(col_dat);
		}
	}

	void CCollider2D::RemoveFromCollisionDetectProcess_() {
		scene_data_ref().collision_world.Remove(this);
	}

	const Vector2Dd CCollider2D::GetCollisionGlobalCenterPosition() const {
		Vector2Dd relation_position = math::RotationalTransformation(transform2d().rotation_rad(), position_); //ゲームオブジェクトからの相対位置
		relation_position.x *= transform2d().scale().x; //横方向拡大を反映
		relation_position.y *= transform2d().scale().y; //縦方向拡大を反映
		return transform2d().position() + relation_position;
	}

	const double CCollider2D::GetCollisionScale() const {
		return transform2d().scale().x > transform2d().scale().y ? transform2d().scale().y : transform2d().scale().x;
	}

	const double CCollider2D::GetCollisionGlobalRotationRad() const {
		return transform2d().rotation_rad() + rotation_rad_;
	}

	CCollider2D& CCollider2D::collision_group(const std::string& cg) {
		if (is_active()) { //アクティブだったら衝突判定プロセスに変更での変更を行う。
			if (scene_data_ref().collision_world.ChangeCollisionGroup(this, cg)) {
				collision_group_name_ = cg;
			} else {
				PE_LOG_ERROR("衝突グループを", collision_group_name_, "から", cg, "に変更できませんでした。");
			}
		} else {
			collision_group_name_ = cg;
		}
		return *this;
	}

	CCollider2D& CCollider2D::collide_with_ground_flag(bool flag) {
		if (is_active()) { //アクティブだったら衝突判定プロセスでの変更を行う。
			if (scene_data_ref().collision_world.ChangeCollisionWithGroundFlag(this, flag)) {
				collide_with_ground_flag_ = flag;
			} else {
				PE_LOG_ERROR("地形との衝突フラグを", collide_with_ground_flag_ ? "true" : "false", "から", flag ? "true" : "false", "に変更できませんでした。");
			}
		} else {
			collide_with_ground_flag_ = flag;
		}
		return *this;
	}

	bool CCollider2D::GetOtherComponentsProc(const GOComponentGetter& com_getter) {
		if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
		transform2d_.reset(com_getter.GetComponent<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2Dを取得できませんでした。");
			return false;
		}
		return true;
	}

	util::DelegateConnection CCollider2D::AddCollidedWithCollider2DEventHandler(util::DelegateHandlerAdder<EACollisionWithCollider2D> handler_adder) {
		return handler_adder(collided_event_);
	}

	util::DelegateConnection CCollider2D::AddCollidedWithGround2DEventHandler(util::DelegateHandlerAdder<EACollisionWithGround2D> handler_adder) {
		return handler_adder(collided_with_ground_event_);
	}

}