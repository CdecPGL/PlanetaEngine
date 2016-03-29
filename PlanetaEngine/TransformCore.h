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

		/*アクセサ*/
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

		/*地形関係*/
		virtual components::GroundComponent& ground() = 0;
		virtual utility::WeakPointer<components::GroundComponent> GetGround()const = 0;
		/*地形をセット(新しい地形、グローバル座標を維持するかどうか[true:グローバル座標が維持される,false:地形座標が維持される])*/
		virtual void SetGround(const utility::WeakPointer<components::GroundComponent>& g, bool keep_global_position) = 0;

		virtual void SetParent(TransformCore* parent) = 0;
	};
}