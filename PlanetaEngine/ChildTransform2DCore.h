#pragma once

#include "Transform2DCore.h"

namespace planeta_engine {
	class ChildTransform2DCore final : public Transform2DCore{
	public:
		/*アクセサ*/
		const Vector2Dd& position()const override;
		void position(const Vector2Dd& pos)override;
		const Vector2Dd& scale()const override;
		void  scale(const Vector2Dd& s)override;
		const double rotation_rad()const override;
		void  rotation_rad(double rota_rad)override;

		const Vector2Dd& global_position()const override;
		void global_position(const Vector2Dd& pos)override;
		const double global_rotation_rad()const override;
		void  global_rotation_rad(double rota_rad)override;

		const Vector2Dd& velocity()const override;
		void  velocity(const Vector2Dd& vel)override;
		const double rotation_velocity_rad()const override;
		void  rotation_velocity_rad(double rota_vel_rad)override;

		const Vector2Dd& global_velocity()const override;
		void global_velocity(const Vector2Dd& vel)override;

		/*地形関係*/
		CGround2D& ground()override;
		/*地形をセット(新しい地形、グローバル座標を維持するかどうか[true:グローバル座標が維持される,false:地形座標が維持される])*/
		utility::WeakPointer<CGround2D> GetGround()const override;
		void SetGround(const utility::WeakPointer<CGround2D>& g, bool keep_global_position)override;

		void SetParent(Transform2DCore* parent)override;
	private:
		void UpdateGlobalFromLocal();
		void UpdateLocalFromGlobal();

		Transform2DCore* parent_ = nullptr;

		TransformData local_transform_data_;
		TransformData global_transform_data_;

	};
}
