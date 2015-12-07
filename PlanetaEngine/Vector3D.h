#pragma once

#include <cstdint>
#include "MathVector.h"

namespace planeta_engine {
	namespace math {
		template<typename T>
		using Vector3D = Vector<T,3>; //�O�����x�N�g��
		using Vector3Df = Vector3D<float>; //�P���x���������_�^�O�����x�N�g��
		using Vector3Dd = Vector3D<double>; //�{���x���������_�^�O�����x�N�g��
		using Vector3Di = Vector3D<int32_t>; //32bit�����t�������^�O�����x�N�g��
	}
}