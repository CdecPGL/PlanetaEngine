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
		/*�Փ˔�����s��*/
		virtual bool DetectCollision(core::IColliderWithCollider2D& collider) = 0;

		/*�O���[�o�����S�ʒu�擾*/
		const Vector2Dd GetCollisionGlobalCenterPosition()const;
		/*�g��x�擾(�g�����X�t�H�[���̊g��xxy�ŏ��������̗v�f���K�p�����)*/
		const double GetCollisionScale()const;
		/*�O���[�o����]�x�擾*/
		const double GetCollisionGlobalRotationRad()const;

		/*�A�N�Z�T*/
		/*���Έʒu���擾*/
		const Vector2Dd& relative_position()const { return position_; }
		/*���Έʒu��ݒ�*/
		CCollider2D& relative_position(const Vector2Dd& pos) { position_ = pos; return *this; }
		/*���Ή�]�x���擾*/
		const double relative_rotation_rad()const { return rotation_rad_; }
		/*���Ή�]�x��ݒ�*/
		CCollider2D& relative_rotation_rad(double rota_rad) { rotation_rad_ = rota_rad; return *this; }
		/*�n�`�Փ˃t���O���擾*/
		const bool collide_with_ground_flag()const { return collide_with_ground_flag_; }
		/*�n�`�Փ˃t���O��ݒ�*/
		CCollider2D& collide_with_ground_flag(bool flag);
		/*�Փ˃O���[�v���擾*/
		const std::string& collision_group()const { return collision_group_name_; }
		/*�Փ˃O���[�v��ݒ�*/
		CCollider2D& collision_group(const std::string& cg);

		CTransform2D& transform2d() { return *transform2d_; }
		const CTransform2D& transform2d()const { return *transform2d_; }

		/*�C�x���g*/
		/*2D�R���C�_�[�Ƃ̏Փ˃C�x���g�n���h���ǉ�*/
		utility::DelegateConnection AddCollidedWithCollider2DEventHandler(utility::DelegateHandlerAdder<EACollisionWithCollider2D> handler_adder);
		/*2D�n�`�Ƃ̏Փ˃C�x���g�n���h���ǉ�*/
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

		/*�ʒu*/
		Vector2Dd position_;
		/*��]�x*/
		double rotation_rad_ = 0.0;
		/*�n�`�Փ˃t���O*/
		bool collide_with_ground_flag_ = false;
		/*�Փ˃O���[�v��*/
		std::string collision_group_name_;
	};
}
