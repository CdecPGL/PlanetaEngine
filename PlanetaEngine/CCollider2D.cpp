#include "CCollider2D.h"
#include "IGameObjectForComponent.h"
#include "CollisionWorld.h"
#include "SystemLog.h"
#include "SceneData.h"
#include "CTransform2D.h"
#include "Matrix2_2.h"
#include "SceneDataForGameObject.h"

namespace planeta_engine {
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
			debug::SystemLog::instance().LogError("衝突グループが設定されていません。", __FUNCTION__);
		} else {
			scene_data_ref().collision_world.Resist(std::static_pointer_cast<CCollider2D>(this_shared()));
		}
	}

	void CCollider2D::RemoveFromCollisionDetectProcess_() {
		scene_data_ref().collision_world.Remove(this);
	}

	const Vector2Dd CCollider2D::GetCollisionGlobalCenterPosition() const {
		Vector2Dd relation_position = math::RotationalTransformation(transform2d().global_rotation_rad(), position_); //ゲームオブジェクトからの相対位置
		relation_position.x *= transform2d().scale().x; //横方向拡大を反映
		relation_position.y *= transform2d().scale().y; //縦方向拡大を反映
		return transform2d().global_position() + relation_position;
	}

	const double CCollider2D::GetCollisionScale() const {
		return transform2d().scale().x > transform2d().scale().y ? transform2d().scale().y : transform2d().scale().x;
	}

	const double CCollider2D::GetCollisionGlobalRotationRad() const {
		return transform2d().global_rotation_rad() + rotation_rad_;
	}

	CCollider2D& CCollider2D::collision_group(const std::string& cg) {
		if (is_active()) { //アクティブだったら衝突判定プロセスに変更での変更を行う。
			if (scene_data_ref().collision_world.ChangeCollisionGroup(this, cg)) {
				collision_group_name_ = cg;
			} else {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "衝突グループを", collision_group_name_, "から", cg, "に変更できませんでした。");
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
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "地形との衝突フラグを", collide_with_ground_flag_ ? "true" : "false", "から", flag ? "true" : "false", "に変更できませんでした。");
			}
		} else {
			collide_with_ground_flag_ = flag;
		}
		return *this;
	}

	bool CCollider2D::OnInitialized() {
		transform2d_.reset(game_object().GetComponent<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2Dを取得できませんでした。");
			return false;
		}
		return true;
	}

}