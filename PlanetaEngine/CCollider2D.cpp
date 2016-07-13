#include "CCollider2D.h"
#include "IGameObjectForComponent.h"
#include "CollisionWorld.h"
#include "SystemLog.h"
#include "SceneData.h"
#include "CTransform2D.h"
#include "Matrix2_2.h"
#include "Collider2DData.h"

namespace planeta {
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
			PE_LOG_ERROR("�Փ˃O���[�v���ݒ肳��Ă��܂���B");
		} else {
			core::Collider2DData col_dat{ *this,game_object(),*transform2d_
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
		Vector2Dd relation_position = math::RotationalTransformation(transform2d().rotation_rad(), position_); //�Q�[���I�u�W�F�N�g����̑��Έʒu
		relation_position.x *= transform2d().scale().x; //�������g��𔽉f
		relation_position.y *= transform2d().scale().y; //�c�����g��𔽉f
		return transform2d().position() + relation_position;
	}

	const double CCollider2D::GetCollisionScale() const {
		return transform2d().scale().x > transform2d().scale().y ? transform2d().scale().y : transform2d().scale().x;
	}

	const double CCollider2D::GetCollisionGlobalRotationRad() const {
		return transform2d().rotation_rad() + rotation_rad_;
	}

	CCollider2D& CCollider2D::collision_group(const std::string& cg) {
		if (is_active()) { //�A�N�e�B�u��������Փ˔���v���Z�X�ɕύX�ł̕ύX���s���B
			if (scene_data_ref().collision_world.ChangeCollisionGroup(this, cg)) {
				collision_group_name_ = cg;
			} else {
				PE_LOG_ERROR("�Փ˃O���[�v��", collision_group_name_, "����", cg, "�ɕύX�ł��܂���ł����B");
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
				PE_LOG_ERROR("�n�`�Ƃ̏Փ˃t���O��", collide_with_ground_flag_ ? "true" : "false", "����", flag ? "true" : "false", "�ɕύX�ł��܂���ł����B");
			}
		} else {
			collide_with_ground_flag_ = flag;
		}
		return *this;
	}

	bool CCollider2D::OnInitialized() {
		transform2d_.reset(game_object().GetComponent<CTransform2D>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2D���擾�ł��܂���ł����B");
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