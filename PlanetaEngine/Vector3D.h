#pragma once

#include <cstdint>
#include "MathVector.h"

namespace planeta_engine {
	namespace math {
		template<typename T>
		using Vector3D = Vector<T,3>; //三次元ベクトル
		using Vector3Df = Vector3D<float>; //単精度浮動小数点型三次元ベクトル
		using Vector3Dd = Vector3D<double>; //倍精度浮動小数点型三次元ベクトル
		using Vector3Di = Vector3D<int32_t>; //32bit符号付き整数型三次元ベクトル
	}
}