#pragma once

#include <array>

#include "vector_2d.hpp"

namespace plnt {
	namespace math {
		//2*2行列
		template <typename T>
		struct matrix_22 {
			std::array<std::array<T, 2>, 2> lines;

			std::array<T, 2> &operator[](int l) { return lines[l]; }

			constexpr const std::array<T, 2> &operator[](int l) const { return lines[l]; }
		};

		//行列関数
		//線形変換
		template <typename T>
		vector_2d<T> linear_transform(const matrix_22<T> &m, const vector_2d<T> &v) {
			return vector_2d<T>(m[0][0] * v.x + m[1][0] * v.y, m[0][1] * v.x + m[1][1] * v.y);
		}

		//回転変換
		template <typename T>
		vector_2d<T> rotation_transform(const double rota_rad, const vector_2d<T> &v) {
			matrix_22<T> m{};
			m[0][0] = static_cast<T>(cos(rota_rad));
			m[1][0] = -static_cast<T>(sin(rota_rad));
			m[0][1] = static_cast<T>(sin(rota_rad));
			m[1][1] = static_cast<T>(cos(rota_rad));
			return linear_transform(m, v);
		}
	}

	using matrix_22d = math::matrix_22<double>;
	// ReSharper disable once CppInconsistentNaming
	using matrix_22f = math::matrix_22<float>;
	// ReSharper disable once CppInconsistentNaming
	using matrix_22i = math::matrix_22<int32_t>;
}
