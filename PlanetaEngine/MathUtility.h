#pragma once

namespace planeta {
	namespace math {
		/*値が2のべき乗数か調べる*/
		bool is2Pow(unsigned int num);
		/*値以上で最少の2のべき乗数を得る*/
		unsigned int getMin2Pow(unsigned int num);
		/*doubleが0であるか絶対誤差を指定して判定*/
		bool DoubleIsZero(double d, double epsilon = 1e-5);
		/*doubleが等しいか絶対誤差を指定して判定*/
		bool DoubleIsEqual(double d1, double d2, double epsilon = 1e-5);
	}
}