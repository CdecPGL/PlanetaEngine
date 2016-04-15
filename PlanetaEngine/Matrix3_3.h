#pragma once

#include <array>
#include "Vector3D.h"

namespace planeta_engine {
	namespace math {
		//3*3s—ñ
		template<typename T>
		struct Matrix3_3 {
			std::array<std::array<T, 3>, 3> lines;
			std::array<T, 3>& operator[](int l) {
				return lines[l];
			}
			constexpr std::array<T, 3>& operator[](int l)const {
				return lines[l];
			}
		};

		//s—ñŠÖ”
		//üŒ`•ÏŠ·
		template<typename T>
		Vector2D<T> LinearTransformation(const Matrix3_3<T>& m, const Vector3D<T>& v) {
			return Vector3D<T>(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z, m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z, m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);
		}
		//‰ñ“]•ÏŠ·
		/*template<typename T>
		Vector2D<T> RotationalTransformation(double rota_rad, const Vector2D<T>& v) {
			MathMatrix<T, 2, 2> m;
			m[0][0] = (T)cos(rota_rad);
			m[1][0] = -(T)sin(rota_rad);
			m[0][1] = (T)sin(rota_rad);
			m[1][1] = (T)cos(rota_rad);
			return LinearTransformation(m, v);
		}*/
	}

	using Matrix2_2d = math::Matrix2_2<double>;
	using Matrix2_2f = math::Matrix2_2<float>;
	using Matrix2_2i = math::Matrix2_2<int32_t>;
}