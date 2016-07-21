#pragma once

#include <limits>
#include "CGround2D.h"
#include "MathConstant.h"

namespace planeta {
	class CDumyGround2D final : public CGround2D {
	public:
	private:
		double GetAltitudeWithGroundPosition(const Vector2Dd& ground_pos) const override { return -1; }
		Vector2Dd ConvertPositionGlobalToGround(const Vector2Dd& global_pos) const override { return global_pos; }
		Vector2Dd ConvertPositionGroundToGlobal(const Vector2Dd& ground_pos) const override { return ground_pos; }
		Vector2Dd NormalizeGroundVectorWithGroundPosition(const Vector2Dd& ground_pos, const Vector2Dd& ground_vector) const override { return ground_vector; }
		double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2Dd& ground_pos) const override { return 0; }
	};
	PE_REGISTER_GAMEOBJECTCOMPONENT(CDumyGround2D);
}