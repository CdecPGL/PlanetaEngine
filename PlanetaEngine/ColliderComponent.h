#pragma once
#include "Component.h"
#include "WeakPointer.h"
#include "Vector2D.h"
#include <functional>
#include "WeakPointerDelegate.h"

namespace planeta_engine {
	namespace event_arguments {
		class CollisionEventArgument;
		class CollisionWithGroundEventArgument;
	}
	namespace system_processes {
		class CollisionDetectProcess;
	}
	namespace components {
		class ColliderComponent : public game::Component {
		public:
			virtual ~ColliderComponent() = default;
			/*���S�ʒu�擾*/
			const Vector2D<double> GetCollisionCenterPosition()const;
			/*�g��x�擾(�g�����X�t�H�[���̊g��xxy�ŏ��������̗v�f���K�p�����)*/
			const double GetCollisionScale()const;
			/*��]�x�擾*/
			const double GetCollisionRotationRad()const;

			/*�A�N�Z�T*/
			/*�ʒu���擾*/
			const Vector2D<double>& position()const { return position_; }
			/*�ʒu��ݒ�*/
			void position(const Vector2D<double>& pos) { position_ = pos; }
			/*��]�x���擾*/
			const double rotation_rad()const { return rotation_rad_; }
			/*��]�x��ݒ�*/
			void rotation_rad(double rota_rad) { rotation_rad_ = rota_rad; }
			/*�n�`�Փ˃t���O���擾*/
			const bool collide_with_ground_flag()const { return collide_with_ground_flag_; }
			/*�n�`�Փ˃t���O��ݒ�*/
			void collide_with_ground_flag(bool flag) { collide_with_ground_flag_ = flag; }
			/*�Փ˃O���[�v���擾*/
			const std::string& collision_group()const { return collision_group_; }
			/*�Փ˃O���[�v��ݒ�*/
			void collision_group(const std::string& cg) { collision_group_ = cg; }

			/*�C�x���g*/
			/*���̂ƏՓ˂���*/
			utility::WeakPointerDelegate<event_arguments::CollisionEventArgument> collided;
			/*�n�`�ƏՓ˂���*/
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
			virtual void _ResistToCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc) = 0;
			virtual void _RemoveFromCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc) = 0;

			/*�ʒu*/
			Vector2D<double> position_;
			/*��]�x*/
			double rotation_rad_ = 0.0;
			/*�n�`�Փ˃t���O*/
			bool collide_with_ground_flag_ = false;
			/*�Փ˃O���[�v*/
			std::string collision_group_;
		};
	}
}
