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
			/*�Փ˔�����s��*/
			virtual bool DetectCollision(core::IColliderWithCollider& collider) = 0;

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
			ColliderComponent& relative_position(const Vector2Dd& pos) { position_ = pos; return *this; }
			/*���Ή�]�x���擾*/
			const double relative_rotation_rad()const { return rotation_rad_; }
			/*���Ή�]�x��ݒ�*/
			ColliderComponent& relative_rotation_rad(double rota_rad) { rotation_rad_ = rota_rad; return *this; }
			/*�n�`�Փ˃t���O���擾*/
			const bool collide_with_ground_flag()const { return collide_with_ground_flag_; }
			/*�n�`�Փ˃t���O��ݒ�*/
			ColliderComponent& collide_with_ground_flag(bool flag);
			/*�Փ˃O���[�v���擾*/
			const std::string& collision_group()const { return collision_group_name_; }
			/*�Փ˃O���[�v��ݒ�*/
			ColliderComponent& collision_group(const std::string& cg);

			/*�C�x���g*/
			/*���̂ƏՓ˂���*/
			utility::WeakPointerDelegate<event_arguments::CollisionEventArgument> collided;
			/*�n�`�ƏՓ˂���*/
			utility::WeakPointerDelegate<event_arguments::CollisionWithGroundEventArgument> collided_with_ground;

		private:
			/*����Z�b�g�A�b�v*/
			bool SpecialSetUp(const core::SceneDataForGameObject& setup_data)override final;

			bool is_no_update() const override final { return true; }
			bool OnActivated() override final;
			bool OnInactivated() override final;
			utility::WeakPointer<system_processes::CollisionDetectProcess> collision_detect_process_;
			void ResistToCollisionDetectProcess_();
			void RemoveFromCollisionDetectProcess_();

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
}
