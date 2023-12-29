﻿#pragma once

#include <array>

#include "Vector2D.hpp"

namespace plnt {
	namespace math {
		//2*2行列
		template <typename T>
		struct matrix22 {
			std::array<std::array<T, 2>, 2> lines;

			std::array<T, 2> &operator[](int l) { return lines[l]; }

			constexpr const std::array<T, 2> &operator[](int l) const { return lines[l]; }
		};

		//行列関数
		//線形変換
		template <typename T>
		Vector2D<T> linear_transform(const matrix22<T> &m, const Vector2D<T> &v) {
			return Vector2D<T>(m[0][0] * v.x + m[1][0] * v.y, m[0][1] * v.x + m[1][1] * v.y);
		}

		//回転変換
		template <typename T>
		Vector2D<T> rotation_transform(const double rota_rad, const Vector2D<T> &v) {
			matrix22<T> m{};
			m[0][0] = static_cast<T>(cos(rota_rad));
			m[1][0] = -static_cast<T>(sin(rota_rad));
			m[0][1] = static_cast<T>(sin(rota_rad));
			m[1][1] = static_cast<T>(cos(rota_rad));
			return linear_transform(m, v);
		}
	}

	using matrix22d = math::matrix22<double>;
	// ReSharper disable once CppInconsistentNaming
	using matrix22f = math::matrix22<float>;
	// ReSharper disable once CppInconsistentNaming
	using matrix22i = math::matrix22<int32_t>;
}
