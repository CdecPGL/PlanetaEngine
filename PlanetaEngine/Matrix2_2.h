#pragma once

#include <array>
#include "Vector2D.h"

namespace planeta {
	namespace math {
		//2*2s—ñ
		template<typename T>
		struct Matrix2_2 {
			std::array<std::array<T, 2>, 2> lines;
			std::array<T, 2>& operator[](int l) {
				return lines[l];
			}
			constexpr const std::array<T, 2>& operator[](int l)const {
				return lines[l];
			}
		};

		//s—ñŠÖ”
		//üŒ`•ÏŠ·
		template<typename T>
		Vector2D<T> LinearTransformation(const Matrix2_2<T>& m, const Vector2D<T>& v) {
			return Vector2D<T>(m[0][0] * v.x + m[1][0] * v.y, m[0][1] * v.x + m[1][1] * v.y);
		}
		//‰ñ“]•ÏŠ·
		template<typename T>
		Vector2D<T> RotationalTransformation(double rota_rad, const Vector2D<T>& v) {
			Matrix2_2<T> m;
			m[0][0] = (T)cos(rota_rad);
			m[1][0] = -(T)sin(rota_rad);
			m[0][1] = (T)sin(rota_rad);
			m[1][1] = (T)cos(rota_rad);
			return LinearTransformation(m, v);
		}
	}

	using Matrix2_2d = math::Matrix2_2<double>;
	using Matrix2_2f = math::Matrix2_2<float>;
	using Matrix2_2i = math::Matrix2_2<int32_t>;
}
