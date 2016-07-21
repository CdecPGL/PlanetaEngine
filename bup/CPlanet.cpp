#include "CPlanet.h"
#include "MathConstant.h"
#include "SystemLog.h"
#include <cmath>
#include <cassert>

namespace {
	constexpr unsigned int kDefaultSeparation(90);
}

namespace planeta {
	CPlanet::CPlanet() {
		separation(kDefaultSeparation);
	}

	CPlanet& CPlanet::separation(unsigned int s) {
		assert(s != 0);
		if (s == 0) {
			PE_LOG_WARNING("分割数に0が指定されましたが、1に設定します。");
			s = 1;
		}
		_gap.resize(s);
		_separation = s;
		return *this;
	}

	void CPlanet::SetGapByIndex(unsigned int idx, double gap) {
		while (idx < 0) { idx += _separation; }
		idx %= _separation;
		assert(0 <= idx && idx < _separation);
		_gap[idx] = gap;
	}

	double CPlanet::GetGapByRad(double rad) const {
		while (rad < 0.0) { rad += 2.0*math::PI; }
		rad = std::fmod(rad, 2.0*math::PI);
		double pos = rad / (2.0*math::PI / _separation);
		int segment = static_cast<int>(pos);
		double pos_in_segment = pos - segment;
		int sidx = segment;
		int eidx = segment == (int)_separation - 1 ? 0 : segment;

		return _gap[sidx] * (1.0 - pos_in_segment) + _gap[eidx] * pos_in_segment;
	}
}