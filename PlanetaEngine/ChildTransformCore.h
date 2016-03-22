#pragma once

#include "TransformCore.h"

namespace planeta_engine {
	class ChildTransformCore final : public TransformCore{
	public:
		/*�A�N�Z�T*/
		const Vector2D<double>& position()const override;
		void position(const Vector2D<double>& pos)override;
		const Vector2D<double>& scale()const override;
		void  scale(const Vector2D<double>& s)override;
		const double rotation_rad()const override;
		void  rotation_rad(double rota_rad)override;

		const Vector2D<double>& global_position()const override;
		void global_position(const Vector2D<double>& pos)override;
		const double global_rotation_rad()const override;
		void  global_rotation_rad(double rota_rad)override;

		const Vector2D<double>& velocity()const override;
		void  velocity(const Vector2D<double>& vel)override;
		const double rotation_velocity_rad()const override;
		void  rotation_velocity_rad(double rota_vel_rad)override;

		const Vector2D<double>& global_velocity()const override;
		void global_velocity(const Vector2D<double>& vel)override;

		/*�n�`�֌W*/
		components::GroundComponent& ground()override;
		/*�n�`���Z�b�g(�V�����n�`�A�O���[�o�����W���ێ����邩�ǂ���[true:�O���[�o�����W���ێ������,false:�n�`���W���ێ������])*/
		utility::WeakPointer<components::GroundComponent> GetGround()const override;
		void SetGround(const utility::WeakPointer<components::GroundComponent>& g, bool keep_global_position)override;
	};
}
