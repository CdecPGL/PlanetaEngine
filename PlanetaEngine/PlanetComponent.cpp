#include "PlanetComponent.h"
#include "MathConstant.h"
#include "SystemLog.h"
#include <cmath>
#include <cassert>

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
			assert(s != 0);
			if (s == 0) {
				debug::SystemLog::instance().LogWarning("•ªŠ„”‚É0‚ªw’è‚³‚ê‚Ü‚µ‚½‚ªA1‚Éİ’è‚µ‚Ü‚·B", __FUNCTION__);
				s = 1;
			}
			_gap.resize(s);
			_separation = s;
			return *this;
		}

		void PlanetComponent::SetGapByIndex(unsigned int idx, double gap) {
			while (idx < 0) { idx += _separation; }
			idx %= _separation;
			assert(0 <= idx && idx < _separation);
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