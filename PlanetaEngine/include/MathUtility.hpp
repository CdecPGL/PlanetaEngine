#pragma once

#include "MathConstant.hpp"

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
		/*度からラジアンに変換*/
		template<typename T>
		constexpr T DegToRad(const T& deg) {
			return static_cast<T>(static_cast<double>(deg) / 180.0 * PI);
		}
		/*ラジアンから度に変換*/
		template<typename T>
		constexpr T RadToDeg(const T& rad) {
			return static_cast<T>(static_cast<double>(rad) / PI * 180.0);
		}
	}
}