#pragma once

#include "Vector2D.h"
#include "WeakPointer.h"

namespace planeta_engine {
	class CGround2D;
	class Transform2DCore {
	public:
		Transform2DCore() = default;
		virtual ~Transform2DCore() = default;

		virtual void Offset(const Vector2Dd& base_pos, const Vector2Dd& offset) = 0;

		/*アクセサ*/
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

		/*地形関係*/
		virtual CGround2D& ground() = 0;
		virtual utility::WeakPointer<CGround2D> GetGround()const = 0;
		/*地形をセット(新しい地形、グローバル座標を維持するかどうか[true:グローバル座標が維持される,false:地形座標が維持される])*/
		virtual void SetGround(const utility::WeakPointer<CGround2D>& g, bool keep_global_position) = 0;

		virtual void SetParent(Transform2DCore* parent) = 0;
	protected:
		//形状情報
		struct TransformData {
			Vector2Dd position;
			Vector2Dd scale = Vector2Dd(1.0, 1.0);
			double rotation_rad = 0.0;
		};
	};
}