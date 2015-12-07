#pragma once

#include <cstdint>
#include "MathVector.h"

namespace planeta_engine {
	namespace math {
		template<typename T>
		using Vector4D = Vector<T, 4>; //�l�����x�N�g��
		using Vector4Df = Vector4D<float>; //�P���x���������_�^�l�����x�N�g��
		using Vector4Dd = Vector4D<double>; //�{���x���������_�^�l�����x�N�g��
		using Vector4Di = Vector4D<int32_t>; //32bit�����t�������^�l�����x�N�g��
	}
}
