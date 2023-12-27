#pragma once

#include <limits>

#include "..\math\math_constant.hpp"

#include "CGround2D.hpp"

namespace plnt {
	/*! 何もしない地面コンポーネント*/
	class CDumyGround2D final : public CGround2D {
	public:
		using super = CGround2D;

	private:
		double GetAltitudeWithGroundPosition(const Vector2Dd &ground_pos) const override { return -1; }
		Vector2Dd ConvertPositionGlobalToGround(const Vector2Dd &global_pos) const override { return global_pos; }
		Vector2Dd ConvertPositionGroundToGlobal(const Vector2Dd &ground_pos) const override { return ground_pos; }

		Vector2Dd NormalizeGroundVectorWithGroundPosition(const Vector2Dd &ground_pos,
		                                                  const Vector2Dd &ground_vector) const override {
			return ground_vector;
		}

		double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2Dd &ground_pos) const override {
			return 0;
		}
	};

	//PE_GAMEOBJECTCOMPONENT_CLASS(CDumyGround2D);
}
