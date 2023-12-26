#pragma once

#include "math_constant.hpp"

namespace plnt::math {
	/*値が2のべき乗数か調べる*/
	bool is2_pow(unsigned int num);
	/*値以上で最少の2のべき乗数を得る*/
	unsigned int get_min2_pow(unsigned int num);
	/*doubleが0であるか絶対誤差を指定して判定*/
	bool double_is_zero(double d, double epsilon = 1e-5);
	/*doubleが等しいか絶対誤差を指定して判定*/
	bool double_is_equal(double d1, double d2, double epsilon = 1e-5);
	/*度からラジアンに変換*/
	template <typename T>
	constexpr T deg_to_rad(const T &deg) { return static_cast<T>(static_cast<double>(deg) / 180.0 * pi); }

	/*ラジアンから度に変換*/
	template <typename T>
	constexpr T rad_to_deg(const T &rad) { return static_cast<T>(static_cast<double>(rad) / pi * 180.0); }
}
