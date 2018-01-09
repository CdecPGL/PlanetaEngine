#include "MathUtility.hpp"
#include <cmath>

namespace plnt {
	namespace math {

		bool is2Pow(unsigned int num)
		{
			int bit = sizeof(num) * 8;
			bool flag = false;
			for (int i = 0; i < bit; ++i, num >>= 1) {
				if (num & 1) {
					if (flag) {
						return false;
					}
					else {
						flag = true;
					}
				}
			}
			return flag;
		}

		unsigned int getMin2Pow(unsigned int num)
		{
			unsigned int out = 1;
			int bit = sizeof(num) * 8;
			for (int i = 0; i < bit && out < num; ++i, out <<= 1) {
			}
			return out;
		}

		bool DoubleIsZero(double d, double epsilon)
		{
			return std::abs(d) <= epsilon;
		}

		bool DoubleIsEqual(double d1, double d2, double epsilon)
		{
			return DoubleIsZero(d2 - d1, epsilon);
		}

	}
}