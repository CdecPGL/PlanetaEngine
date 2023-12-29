#pragma once

#include <array>

#include "Vector3D.hpp"

namespace plnt {
	namespace math {
		//3*3行列
		template <typename T>
		struct matrix33 {
			std::array<std::array<T, 3>, 3> lines;

			std::array<T, 3> &operator[](int l) { return lines[l]; }

			constexpr std::array<T, 3> &operator[](int l) const { return lines[l]; }
		};

		//行列関数
		//線形変換
		template <typename T>
		Vector3D<T> linear_transform(const matrix33<T> &m, const Vector3D<T> &v) {
			return Vector3D<T>(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
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

	using matrix33d = math::matrix33<double>;
	// ReSharper disable once CppInconsistentNaming
	using matrix33f = math::matrix33<float>;
	// ReSharper disable once CppInconsistentNaming
	using matrix33i = math::matrix33<int32_t>;
}
