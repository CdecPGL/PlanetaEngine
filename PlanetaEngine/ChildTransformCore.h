#pragma once

#include "TransformCore.h"

namespace planeta_engine {
	class ChildTransformCore final : public TransformCore{
	public:
		/*アクセサ*/
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

		/*地形関係*/
		components::GroundComponent& ground()override;
		/*地形をセット(新しい地形、グローバル座標を維持するかどうか[true:グローバル座標が維持される,false:地形座標が維持される])*/
		utility::WeakPointer<components::GroundComponent> GetGround()const override;
		void SetGround(const utility::WeakPointer<components::GroundComponent>& g, bool keep_global_position)override;
	};
}
