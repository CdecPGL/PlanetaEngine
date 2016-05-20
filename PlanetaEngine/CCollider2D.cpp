#include "CCollider2D.h"
#include "IGameObjectAccessor.h"
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
			debug::SystemLog::instance().LogError("�Փ˃O���[�v���ݒ肳��Ă��܂���B", __FUNCTION__);
		} else {
			scene_data_ref().collision_world.Resist(std::static_pointer_cast<CCollider2D>(this_shared()));
		}
	}

	void CCollider2D::RemoveFromCollisionDetectProcess_() {
		scene_data_ref().collision_world.Remove(this);
	}

	const Vector2Dd CCollider2D::GetCollisionGlobalCenterPosition() const {
		const CTransform2D& transform = game_object().transform();
		Vector2Dd relation_position = math::RotationalTransformation(transform.global_rotation_rad(), position_); //�Q�[���I�u�W�F�N�g����̑��Έʒu
		relation_position.x *= transform.scale().x; //�������g��𔽉f
		relation_position.y *= transform.scale().y; //�c�����g��𔽉f
		return transform.global_position() + relation_position;
	}

	const double CCollider2D::GetCollisionScale() const {
		const CTransform2D& transform = game_object().transform();
		return transform.scale().x > transform.scale().y ? transform.scale().y : transform.scale().x;
	}

	const double CCollider2D::GetCollisionGlobalRotationRad() const {
		return game_object().transform().global_rotation_rad() + rotation_rad_;
	}

	CCollider2D& CCollider2D::collision_group(const std::string& cg) {
		if (is_active()) { //�A�N�e�B�u��������Փ˔���v���Z�X�ɕύX�ł̕ύX���s���B
			if (scene_data_ref().collision_world.ChangeCollisionGroup(this, cg)) {
				collision_group_name_ = cg;
			} else {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�Փ˃O���[�v��", collision_group_name_, "����", cg, "�ɕύX�ł��܂���ł����B");
			}
		} else {
			collision_group_name_ = cg;
		}
		return *this;
	}

	CCollider2D& CCollider2D::collide_with_ground_flag(bool flag) {
		if (is_active()) { //�A�N�e�B�u��������Փ˔���v���Z�X�ł̕ύX���s���B
			if (scene_data_ref().collision_world.ChangeCollisionWithGroundFlag(this, flag)) {
				collide_with_ground_flag_ = flag;
			} else {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�n�`�Ƃ̏Փ˃t���O��", collide_with_ground_flag_ ? "true" : "false", "����", flag ? "true" : "false", "�ɕύX�ł��܂���ł����B");
			}
		} else {
			collide_with_ground_flag_ = flag;
		}
		return *this;
	}
}