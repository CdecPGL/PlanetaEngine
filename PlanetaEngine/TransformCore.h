#pragma once

#include "Vector2D.h"
#include "WeakPointer.h"

namespace planeta_engine {
	namespace components {
		class GroundComponent;
	}
	class TransformCore {
	public:
		TransformCore() = default;
		virtual ~TransformCore() = default;

		virtual void Offset(const Vector2D<double>& base_pos, const Vector2D<double>& offset) = 0;

		/*�A�N�Z�T*/
		virtual const Vector2D<double>& position()const = 0;
		virtual void position(const Vector2D<double>& pos) = 0;
		virtual const Vector2D<double>& scale()const = 0;
		virtual void  scale(const Vector2D<double>& s) = 0;
		virtual const double rotation_rad()const = 0;
		virtual void  rotation_rad(double rota_rad) = 0;

		virtual const Vector2D<double>& global_position()const = 0;
		virtual void global_position(const Vector2D<double>& pos) = 0;
		virtual const double global_rotation_rad()const = 0;
		virtual void  global_rotation_rad(double rota_rad) = 0;

		virtual const Vector2D<double>& velocity()const = 0;
		virtual void  velocity(const Vector2D<double>& vel) = 0;
		virtual const double rotation_velocity_rad()const = 0;
		virtual void  rotation_velocity_rad(double rota_vel_rad) = 0;

		virtual const Vector2D<double>& global_velocity()const = 0;
		virtual void global_velocity(const Vector2D<double>& vel) = 0;

		/*�n�`�֌W*/
		virtual components::GroundComponent& ground() = 0;
		virtual utility::WeakPointer<components::GroundComponent> GetGround()const = 0;
		/*�n�`���Z�b�g(�V�����n�`�A�O���[�o�����W���ێ����邩�ǂ���[true:�O���[�o�����W���ێ������,false:�n�`���W���ێ������])*/
		virtual void SetGround(const utility::WeakPointer<components::GroundComponent>& g, bool keep_global_position) = 0;

		virtual void SetParent(TransformCore* parent) = 0;
	};
}