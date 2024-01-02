#pragma once

#include <array>

#include "vector_3d.hpp"

namespace plnt {
	namespace math {
		//3*3行列
		template <typename T>
		struct matrix_33 {
			std::array<std::array<T, 3>, 3> lines;

			std::array<T, 3> &operator[](int l) { return lines[l]; }

			constexpr std::array<T, 3> &operator[](int l) const { return lines[l]; }
		};

		//行列関数
		//線形変換
		template <typename T>
		vector_3d<T> linear_transform(const matrix_33<T> &m, const vector_3d<T> &v) {
			return vector_3d<T>(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
			                   m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
			                   m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);
		}

		//回転変換
		/*template<typename T>
		Vector2D<T> rotation_transform(const double rota_rad, const Vector2D<T>& v) {
			MathMatrix<T, 2, 2> m{};
			m[0][0] = (T)cos(rota_rad);
			m[1][0] = -(T)sin(rota_rad);
			m[0][1] = (T)sin(rota_rad);
			m[1][1] = (T)cos(rota_rad);
			return linear_transform(m, v);
		}*/
	}

	using matrix_33d = math::matrix_33<double>;
	// ReSharper disable once CppInconsistentNaming
	using matrix_33f = math::matrix_33<float>;
	// ReSharper disable once CppInconsistentNaming
	using matrix_33i = math::matrix_33<int32_t>;
}
