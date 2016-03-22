#pragma once

#include <unordered_map>
#include "GameObjectNormalComponent.h"
#include "Vector2D.h"
#include "WeakPointerDelegate.h"

namespace planeta_engine {
	class TransformCore;
	namespace components {
		class GroundComponent;
		/*�`����R���|�[�l���g*/
		class TransformComponent : public GameObjectNormalComponent {
		public:
			TransformComponent();
			~TransformComponent();

			/*���݈ʒu����ړ�*/
			void Move(const Vector2D<double>& mov_pos);
			/*�ʒu�̐ݒ�*/
			void Offset(const Vector2D<double>& base_pos, const Vector2D<double>& offset);
			/*��������*/
			void Accelerate(const Vector2D<double>& acceleration);
			/*�O���[�o�������ɉ�������*/
			void GlobalAccelerate(const Vector2D<double>& global_acceleration);
			/*�A�N�Z�T*/
			const Vector2D<double>& position()const;
			TransformComponent& position(const Vector2D<double>& pos);
			const Vector2D<double>& scale()const;
			TransformComponent&  scale(const Vector2D<double>& s);
			const double rotation_rad()const;
			TransformComponent&  rotation_rad(double rota_rad);

			const Vector2D<double>& global_position()const;
			TransformComponent&  global_position(const Vector2D<double>& pos);
			const double global_rotation_rad()const;
			TransformComponent&  global_rotation_rad(double rota_rad);

			const Vector2D<double>& velocity()const;
			TransformComponent&  velocity(const Vector2D<double>& vel);
			const double rotation_velocity_rad()const;
			TransformComponent&  rotation_velocity_rad(double rota_vel_rad);

			const Vector2D<double>& global_velocity()const;
			TransformComponent&  global_velocity(const Vector2D<double>& vel);

			/*�n�`�֌W*/
			const GroundComponent& ground()const;
			GroundComponent& ground();
			/*�n�`���Z�b�g(�V�����n�`�A�O���[�o�����W���ێ����邩�ǂ���[true:�O���[�o�����W���ێ������,false:�n�`���W���ێ������])*/
			utility::WeakPointer<GroundComponent> GetGround()const;
			void SetGround(const utility::WeakPointer<GroundComponent>& g,bool keep_global_position);

			//�V�X�e��
			void ApplyVelocity();
		private:
			bool is_no_update()const override{ return true; }

			std::unique_ptr<TransformCore> core_;
		};
	}
}
