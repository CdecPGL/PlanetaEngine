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
			void Move(const Vector2Dd& mov_pos);
			/*�ʒu�̐ݒ�*/
			void Offset(const Vector2Dd& base_pos, const Vector2Dd& offset);
			/*��������*/
			void Accelerate(const Vector2Dd& acceleration);
			/*�O���[�o�������ɉ�������*/
			void GlobalAccelerate(const Vector2Dd& global_acceleration);
			/*�A�N�Z�T*/
			const Vector2Dd& position()const;
			TransformComponent& position(const Vector2Dd& pos);
			const Vector2Dd& scale()const;
			TransformComponent&  scale(const Vector2Dd& s);
			const double rotation_rad()const;
			TransformComponent&  rotation_rad(double rota_rad);

			const Vector2Dd& global_position()const;
			TransformComponent&  global_position(const Vector2Dd& pos);
			const double global_rotation_rad()const;
			TransformComponent&  global_rotation_rad(double rota_rad);

			const Vector2Dd& velocity()const;
			TransformComponent&  velocity(const Vector2Dd& vel);
			const double rotation_velocity_rad()const;
			TransformComponent&  rotation_velocity_rad(double rota_vel_rad);

			const Vector2Dd& global_velocity()const;
			TransformComponent&  global_velocity(const Vector2Dd& vel);

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
