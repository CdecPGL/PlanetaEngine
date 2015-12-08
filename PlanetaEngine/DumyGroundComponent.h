#pragma once

#include <limits>
#include "GroundComponent.h"
#include "MathConstant.h"

namespace planeta_engine {
	namespace components {
		class DumyGroundComponent final: public GroundComponent {
		public:
		private:
			const math::Vector2Dd GetDownwardDirectionVector(const math::Vector2Dd& pos)const override { return ConvertRadToVec(GetDownwardDirectionByRad(pos)); }
			const double GetDownwardDirectionByRad(const math::Vector2Dd& pos)const override { return math::constant::PI*1.5; }
			const double GetAltitude(const math::Vector2Dd& position)const { return std::numeric_limits<double>().max(); }
		};
	}
}