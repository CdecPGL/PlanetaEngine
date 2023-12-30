#pragma once

#include <cstdint>
#include <string>

#include "boost/property_tree/ptree.hpp"

#include "../reflection/reflection_utility.hpp"

namespace plnt {
	template <typename T>
	struct vector_2d;
	template <typename T>
	struct vector_3d;

	//三次元ベクトルテンプレート
	template <typename T>
	struct vector_4d {
		//要素
		T x, y, z, w;
		//コンストラクタ
		constexpr vector_4d() : x(0), y(0), z(0), w(0) {}
		constexpr vector_4d(T m_x, T m_y, T m_z, T m_w) : x(m_x), y(m_y), z(m_z), w(m_w) {}
		constexpr vector_4d(const vector_4d &obj) = default;
		constexpr vector_4d(vector_4d &&obj) = default;

		template <typename T2>
		explicit constexpr vector_4d(const vector_4d<T2> &obj) : x(static_cast<T>(obj.x)), y(static_cast<T>(obj.y)),
		                                                       z(static_cast<T>(obj.z)), w(static_cast<T>(obj.w)) {}

		explicit constexpr vector_4d(const vector_2d<T> &v2) : x(v2.x), y(v2.y), z(0), w(0) {}
		explicit constexpr vector_4d(const vector_3d<T> &v3) : x(v3.x), y(v3.y), z(v3.z), w(0) {}

		constexpr ~vector_4d() = default;

		//代入演算子
		constexpr vector_4d &operator =(const vector_4d &obj) {
			x = obj.x;
			y = obj.y;
			z = obj.z;
			w = obj.w;
			return *this;
		}

		// ムーブ演算子
		constexpr vector_4d &operator=(vector_4d &&obj) = default;

		//キャスト演算子
		template <typename T2>
		explicit constexpr operator vector_4d<T2>() const {
			return vector_4d<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z), static_cast<T2>(w));
		}

		//加減算演算子
		constexpr vector_4d operator+(const vector_4d &in) const {
			return Vector4D(x + in.x, y + in.y, z + in.z, w + in.w);
		}

		constexpr vector_4d operator-(const vector_4d &in) const {
			return Vector4D(x - in.x, y - in.y, z - in.z, w - in.w);
		}

		//スカラーとの乗除算演算子
		template <typename U>
		constexpr vector_4d operator*(U num) const {
			return Vector4D(static_cast<T>(x * num), static_cast<T>(y * num), static_cast<T>(z * num),
			                static_cast<T>(w * num));
		}

		template <typename U>
		constexpr vector_4d operator/(U num) const {
			if (num == 0) { throw std::range_error("Divided by zero."); }
			vector_4d out(static_cast<T>(static_cast<double>(x) / num), static_cast<T>(static_cast<double>(y) / num),
			             static_cast<T>(static_cast<double>(z) / num), static_cast<T>(static_cast<double>(w) / num));
			return out;
		}

		//加減代入演算子
		constexpr vector_4d &operator+=(const vector_4d &in) {
			x += in.x;
			y += in.y;
			z += in.z;
			w += in.w;
			return *this;
		}

		constexpr vector_4d &operator-=(const vector_4d &in) {
			x -= in.x;
			y -= in.y;
			z -= in.z;
			w -= in.w;
			return *this;
		}

		//スカラーとの乗除算代入演算子
		template <typename U>
		constexpr vector_4d operator*=(U num) {
			x *= num;
			y *= num;
			z *= num;
			w *= num;
			return *this;
		}

		template <typename U>
		constexpr vector_4d operator/=(U num) {
			if (num == 0) { throw std::range_error("Divided by zero."); }
			x /= num;
			y /= num;
			z /= num;
			w /= num;
			return *this;
		}

		//符号反転演算子
		constexpr vector_4d operator-() const { return Vector4D(-x, -y, -z, -w); }

		//比較演算子（!=は自動導出）
		constexpr bool operator==(const vector_4d &in) const = default;

		//その他関数
		constexpr void set(T mx, T my, T mz, T mw) {
			x = mx;
			y = my;
			z = mz;
			w = mw;
		}

		//平行な単位ベクトルを求める(同じ向きのもの)
		[[nodiscard]] constexpr vector_4d get_parallel_unit_vector() {
			const double v_length(length());
			// NOLINTNEXTLINE(clang-diagnostic-float-equal)
			if (v_length == 0) { throw std::range_error("tried to calc unit vector from zero vector"); }
			return operator/(v_length);
		}

		// v1の四捨五入値ベクトルを取得
		[[nodiscard]] constexpr vector_4d get_round_vector() const {
			return Vector4D(static_cast<T>(round(static_cast<double>(x))),
			                static_cast<T>(round(static_cast<double>(y))),
			                static_cast<T>(round(static_cast<double>(z))),
			                static_cast<T>(round(static_cast<double>(w))));
		}

		[[nodiscard]] constexpr double length() const { return sqrt(length_square()); }

		[[nodiscard]] constexpr double length_square() const { return x * x + y * y + z * z + w * w; }

		[[nodiscard]] std::string to_string() const {
			using namespace std;
			stringstream is;
			is << "(" << x << "," << y << "," << z << "," << w << ")";
			return is.str();
		}

		//定義済みベクトル
		static const vector_4d zero_vector;
		static const vector_4d x_unit_vector;
		static const vector_4d y_unit_vector;
		static const vector_4d z_unit_vector;
		static const vector_4d w_unit_vector;
	};

	template <typename T>
	const vector_4d<T> vector_4d<T>::zero_vector(0, 0, 0, 0);
	template <typename T>
	const vector_4d<T> vector_4d<T>::x_unit_vector(1, 0, 0, 0);
	template <typename T>
	const vector_4d<T> vector_4d<T>::y_unit_vector(0, 1, 0, 0);
	template <typename T>
	const vector_4d<T> vector_4d<T>::z_unit_vector(0, 0, 1, 0);
	template <typename T>
	const vector_4d<T> vector_4d<T>::w_unit_vector(0, 0, 0, 1);

	//四次元ベクトル関数
	//内積を求める
	template <typename T>
	constexpr auto dot(const vector_4d<T> &v1, const vector_4d<T> &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	//v2がv1となす角のコサイン値を求める(時計周りが正)
	template <typename T>
	constexpr double get_cos(const vector_4d<T> &v1, const vector_4d<T> &v2) {
		const double v1_length(v1.length());
		const double v2_length(v2.length());
		const double v1_v2_dot(dot(v1, v2));
		return v1_v2_dot / (v1_length * v2_length);
	}

	//v2がv1となす角のサイン値を求める(時計周りが正)
	/*template<typename T>
	double GetSin(const Vector3D<T>& v1, const Vector3D<T>& v2) {
	double v1_length(v1.length()), v2_length(v2.length());
	double v1v2_cross(Cross(v1, v2));
	return v1v2_cross / (v1_length*v2_length);
	}*/
	//v2がv1となす角の角度(ラジアン)を求める(時計周りが正)
	/*template<typename T>
	double GetRadian(const Vector3D<T>& v1, const Vector3D<T>& v2) {
	double v1v2_cos(get_cos(v1, v2));
	double v1v1_sin(GetSin(v1, v2));
	double v1v2_rad(acos(v1v2_cos));
	if (v1v1_sin < 0) { v1v2_rad = -v1v2_rad; }
	return v1v2_rad;
	}*/
	//v2がv1となす角の角度(度)を求める(時計周りが正)
	/*template<typename T>
	double GetDegree(const Vector3D<T>& v1, const Vector3D<T>& v2) {
	double v1v2_rad = (GetRadian(v1, v2));
	double v1v2_deg(v1v2_rad / math::PI * 180);
	return v1v2_deg;
	}*/

	//v1のv2に対する正射影ベクトルを求める
	template <typename T>
	[[nodiscard]] constexpr vector_4d<T> get_orthogonal_projection_vector(const vector_4d<T> &v1, const vector_4d<T> &v2) {
		return v2.get_parallel_unit_vector() * vector_4d<T>::dot(v1, v2);
	}

	using vector_4df = vector_4d<float>; //単精度浮動小数点型四次元ベクトル
	using vector_4dd = vector_4d<double>; //倍精度浮動小数点型四次元ベクトル
	using vector_4di = vector_4d<int32_t>; //32bit符号付き整数型四次元ベクトル

	//ReflectionシステムのPtree読み込みを有効にするための定義
	template <typename T>
	struct reflection::reflective_ptree_converter_impl<vector_4d<T>> {
		void operator()(vector_4d<T> &dst, const boost::property_tree::ptree &src) {
			if (src.size() != 4) {
				throw reflection_error(
					util::ConvertAndConnectToString("要素数が", src.size(), "ですが、Vector4Dでは4である必要があります。"));
			}
			size_t idx = 0;
			std::array<T, 4> ary;
			for (const auto &[key, value] : src) {
				if (key.empty() == false) {
					throw reflection_error(
						util::ConvertAndConnectToString("Vector4DのPtreeキーは空である必要があります。(読み取られたキー:", key, ")"));
				}
				T dat{};
				reflective_ptree_converter(dat, value);
				ary[idx++] = std::move(dat);
			}
			dst.set(ary[0], ary[1], ary[2], ary[3]);
		}
	};
}
