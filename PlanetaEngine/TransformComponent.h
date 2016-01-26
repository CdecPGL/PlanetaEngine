#pragma once

#include <unordered_map>
#include "GameObjectNormalComponent.h"
#include "Vector2D.h"
#include "WeakPointerDelegate.h"

namespace planeta_engine {
	namespace components {
		class GroundComponent;
		/*�`����R���|�[�l���g*/
		class TransformComponent : public GameObjectNormalComponent {
		public:
			TransformComponent();
			~TransformComponent() = default;

			/*���݈ʒu����ړ�*/
			void Move(const Vector2D<double>& mov_pos);
			/*�ʒu�̐ݒ�*/
			void Offset(const Vector2D<double>& base_pos, const Vector2D<double>& offset);
			/*�A�N�Z�T*/
			const Vector2D<double>& position()const;
			void position(const Vector2D<double>& pos);
			const Vector2D<double>& scale()const;
			void scale(const Vector2D<double>& s);
			const double rotation_rad()const;
			void rotation_rad(double rota_rad);

			const Vector2D<double>& global_position()const;
			void global_position(const Vector2D<double>& pos);
			const double global_rotation_rad()const;
			void global_rotation_rad(double rota_rad);

			const Vector2D<double>& velocity()const;
			void velocity(const Vector2D<double>& vel);
			const double rotation_velocity_rad()const;
			void rotation_velocity_rad(double rota_vel_rad);

			const Vector2D<double>& global_velocity()const;
			void global_velocity(const Vector2D<double>& vel);

			//�V�X�e��
			void ApplyVelocity();
		private:
			bool is_no_update()const override{ return true; }

			void UpdateTransformDataGlobalByGround(); //�n�`���W����O���[�o�����W���Z�o
			void UpdateTransformDataGroundByGlobal(); //�n�`���W���烍�[�J�����W���Z�o
			void UpdatePhysicalDataGlobal(); //�n�`���W����O���[�o�����W���Z�o
			void UpdatePhysicalDataGround(); //�n�`���W���烍�[�J�����W���Z�o

			utility::WeakPointer<GroundComponent> belonging_ground_;

			//�`����
			struct TransformData {
				Vector2D<double> position;
				Vector2D<double> scale = Vector2D<double>(1.0, 1.0);
				double rotation_rad = 0.0;
				bool position_updated = false;
				bool rotation_updated = false;
			};
			TransformData ground_transform_data_;
			TransformData global_transform_data_;

			//�������
			Vector2D<double> ground_velocity_; //�n�`���W�n�ł̑��x�B�����̓O���[�o�����W�n�ɏ�����
			bool ground_velocity_updated_ = false;
			Vector2D<double> global_velocity_;
			bool global_velocity_updated_ = false;
			double rotation_velocity_rad_;
		};
	}
}
