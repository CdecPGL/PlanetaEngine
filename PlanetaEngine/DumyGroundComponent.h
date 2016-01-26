#pragma once

#include <limits>
#include "GroundComponent.h"
#include "MathConstant.h"

namespace planeta_engine {
	namespace components {
		class DumyGroundComponent final: public GroundComponent {
		public:
		private:
			double GetAltitudeWithGroundPosition(const Vector2D<double>& ground_pos) const override { return -1; }
			Vector2D<double> ConvertPositionGlobalToGround(const Vector2D<double>& global_pos) const override { return global_pos; }
			Vector2D<double> ConvertPositionGroundToGlobal(const Vector2D<double>& ground_pos) const override { return ground_pos; }
			Vector2D<double> NormalizeGroundVectorWithGroundPosition(const Vector2D<double>& ground_pos, const Vector2D<double>& ground_vector) const override { return ground_vector; }
			double GetAngleDifferenceInRadGroundFromGlobalWithGroundPosition(const Vector2D<double>& ground_pos) const override { return 0; }

		};
	}
}