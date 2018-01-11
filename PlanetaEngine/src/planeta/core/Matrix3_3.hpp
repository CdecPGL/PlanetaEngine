#pragma once

#include <array>
#include "Vector3D.hpp"

namespace plnt {
	namespace math {
		//3*3行列
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

		//行列関数
		//線形変換
		template<typename T>
		Vector3D<T> LinearTransformation(const Matrix3_3<T>& m, const Vector3D<T>& v) {
			return Vector3D<T>(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z, m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z, m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);
		}
		//回転変換
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

	using Matrix3_3d = math::Matrix3_3<double>;
	using Matrix3_3f = math::Matrix3_3<float>;
	using Matrix3_3i = math::Matrix3_3<int32_t>;
}