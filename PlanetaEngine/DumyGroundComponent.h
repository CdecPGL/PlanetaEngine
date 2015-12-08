#pragma once

#include <limits>
#include "GroundComponent.h"
#include "MathConstant.h"

namespace planeta_engine {
	namespace components {
		class DumyGroundComponent final: public GroundComponent {
		public:
		private:
			const Vector2D<double> GetDownwardDirectionVector(const Vector2D<double>& pos)const override { return ConvertRadToVec(GetDownwardDirectionByRad(pos)); }
			const double GetDownwardDirectionByRad(const Vector2D<double>& pos)const override { return math::constant::PI*1.5; }
			const double GetAltitude(const Vector2D<double>& position)const { return std::numeric_limits<double>().max(); }
		};
	}
}