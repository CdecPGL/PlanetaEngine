#include "PlanetComponent.h"
#include "MathConstant.h"
#include <cmath>

namespace {
	constexpr unsigned int kDefaultSeparation(90);
}

namespace planeta_engine {
	namespace components {

		PlanetComponent::PlanetComponent()
		{
			separation(kDefaultSeparation);
		}

		PlanetComponent& PlanetComponent::separation(unsigned int s)
		{
			_gap.resize(s);
			_separation = s;
			return *this;
		}

		void PlanetComponent::SetGapByIndex(unsigned int idx, double gap)
		{
			while (idx < 0) { idx += _separation; }
			idx %= _separation;
			_gap[idx] = gap;
		}

		double PlanetComponent::GetGapByRad(double rad) const
		{
			while (rad < 0.0) { rad += 2.0*math::constant::PI; }
			rad = std::fmod(rad, 2.0*math::constant::PI);
			double pos = rad / (2.0*math::constant::PI / _separation);
			int segment = static_cast<int>(pos);
			double pos_in_segment = pos - segment;
			int sidx = segment;
			int eidx = segment == (int)_separation - 1 ? 0 : segment;

			return _gap[sidx] * (1.0 - pos_in_segment) + _gap[eidx] * pos_in_segment;
		}

	}
}