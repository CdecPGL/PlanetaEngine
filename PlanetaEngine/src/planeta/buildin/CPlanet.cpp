#include <cmath>
#include <cassert>

#include "../math/math_constant.hpp"
#include "../core/log_utility.hpp"
#include "CPlanet.hpp"

namespace {
	constexpr unsigned int default_separation(90);
}

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_planet) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_planet, radius)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_planet, separation)
			.shallow_copy_target(&c_planet::radius_)
			.shallow_copy_target(&c_planet::separation_)
			.shallow_copy_target(&c_planet::gap_);
	}

	c_planet::c_planet() { separation(default_separation); }

	c_planet &c_planet::separation(unsigned int s) {
		assert(s != 0);
		if (s == 0) {
			PE_LOG_WARNING("分割数に0が指定されましたが、1に設定します。");
			s = 1;
		}
		gap_.resize(s);
		separation_ = s;
		return *this;
	}

	void c_planet::set_gap_by_index(unsigned int idx, const double gap) {
		idx %= separation_;
		assert(idx < separation_);
		gap_[idx] = gap;
	}

	double c_planet::get_gap_by_rad(double rad) const {
		while (rad < 0.0) { rad += 2.0 * math::pi; }
		rad = std::fmod(rad, 2.0 * math::pi);
		const double pos = rad / (2.0 * math::pi / separation_);
		const int segment = static_cast<int>(pos);
		const double pos_in_segment = pos - segment;
		const int s_idx = segment;
		const int e_idx = segment == static_cast<int>(separation_) - 1 ? 0 : segment;

		return gap_[s_idx] * (1.0 - pos_in_segment) + gap_[e_idx] * pos_in_segment;
	}
}
