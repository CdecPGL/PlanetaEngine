#pragma once

#include <cstdint>
#include "MathVector.h"

namespace planeta_engine {
	namespace math {
		template<typename T>
		using Vector4D = Vector<T, 4>; //四次元ベクトル
		using Vector4Df = Vector4D<float>; //単精度浮動小数点型四次元ベクトル
		using Vector4Dd = Vector4D<double>; //倍精度浮動小数点型四次元ベクトル
		using Vector4Di = Vector4D<int32_t>; //32bit符号付き整数型四次元ベクトル
	}
}
