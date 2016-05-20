#include "CCollider2D.h"
#include "IGameObjectAccessor.h"
#include "TCollisionDetect.h"
#include "SystemLog.h"
#include "SceneAccessorForGameObject.h"
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
		if (collision_detect_process_) {
			if (collision_group_name_.length() == 0) {
				debug::SystemLog::instance().LogError("�Փ˃O���[�v���ݒ肳��Ă��܂���B", __FUNCTION__);
			} else {
				collision_detect_process_->Resist(std::static_pointer_cast<CCollider2D>(this_shared()));
			}
		} else {
			debug::SystemLog::instance().LogError("�Փ˔���v���Z�X���擾�ł��Ă��܂���B", __FUNCTION__);
		}
	}

	void CCollider2D::RemoveFromCollisionDetectProcess_() {
		if (collision_detect_process_) {
			collision_detect_process_->Remove(this);
		} else {
			debug::SystemLog::instance().LogError("�Փ˔���v���Z�X���擾�ł��Ă��܂���B", __FUNCTION__);
		}
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
			assert(collision_detect_process_ != nullptr);
			if (collision_detect_process_->ChangeCollisionGroup(this, cg)) {
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
			assert(collision_detect_process_ != nullptr);
			if (collision_detect_process_->ChangeCollisionWithGroundFlag(this, flag)) {
				collide_with_ground_flag_ = flag;
			} else {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, __FUNCTION__, "�n�`�Ƃ̏Փ˃t���O��", collide_with_ground_flag_ ? "true" : "false", "����", flag ? "true" : "false", "�ɕύX�ł��܂���ł����B");
			}
		} else {
			collide_with_ground_flag_ = flag;
		}
		return *this;
	}

	bool CCollider2D::SpecialSetUp(const core::SceneDataForGameObject& setup_data) {
		collision_detect_process_ = setup_data.collision_detect_process;
		return true;
	}
}