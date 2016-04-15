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

		virtual void Offset(const Vector2Dd& base_pos, const Vector2Dd& offset) = 0;

		/*�A�N�Z�T*/
		virtual const Vector2Dd& position()const = 0;
		virtual void position(const Vector2Dd& pos) = 0;
		virtual const Vector2Dd& scale()const = 0;
		virtual void  scale(const Vector2Dd& s) = 0;
		virtual const double rotation_rad()const = 0;
		virtual void  rotation_rad(double rota_rad) = 0;

		virtual const Vector2Dd& global_position()const = 0;
		virtual void global_position(const Vector2Dd& pos) = 0;
		virtual const double global_rotation_rad()const = 0;
		virtual void  global_rotation_rad(double rota_rad) = 0;

		virtual const Vector2Dd& velocity()const = 0;
		virtual void  velocity(const Vector2Dd& vel) = 0;
		virtual const double rotation_velocity_rad()const = 0;
		virtual void  rotation_velocity_rad(double rota_vel_rad) = 0;

		virtual const Vector2Dd& global_velocity()const = 0;
		virtual void global_velocity(const Vector2Dd& vel) = 0;

		/*�n�`�֌W*/
		virtual components::GroundComponent& ground() = 0;
		virtual utility::WeakPointer<components::GroundComponent> GetGround()const = 0;
		/*�n�`���Z�b�g(�V�����n�`�A�O���[�o�����W���ێ����邩�ǂ���[true:�O���[�o�����W���ێ������,false:�n�`���W���ێ������])*/
		virtual void SetGround(const utility::WeakPointer<components::GroundComponent>& g, bool keep_global_position) = 0;

		virtual void SetParent(TransformCore* parent) = 0;
	protected:
		//�`����
		struct TransformData {
			Vector2Dd position;
			Vector2Dd scale = Vector2Dd(1.0, 1.0);
			double rotation_rad = 0.0;
		};
	};
}