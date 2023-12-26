#include <cmath>

#include "math_utility.hpp"

namespace plnt::math {
	bool is2_pow(unsigned int num) {
		constexpr int bit = sizeof(num) * 8;
		bool flag = false;
		for (int i = 0; i < bit; ++i, num >>= 1) { if (num & 1) { if (flag) { return false; } else { flag = true; } } }
		return flag;
	}

	unsigned int get_min2_pow(const unsigned int num) {
		unsigned int out = 1;
		constexpr int bit = sizeof(num) * 8;
		for (int i = 0; i < bit && out < num; ++i, out <<= 1) { }
		return out;
	}

	bool double_is_zero(const double d, const double epsilon) { return std::abs(d) <= epsilon; }

	bool double_is_equal(const double d1, const double d2, const double epsilon) {
		return double_is_zero(d2 - d1, epsilon);
	}
}
