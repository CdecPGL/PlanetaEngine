#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <stdexcept>
#include <cmath>

#include "boost/property_tree/ptree.hpp"

#include "../math/math_constant.hpp"
#include "../reflection/reflection_exceptions.hpp"
#include "string_utility.hpp"
#include "../reflection/reflection_utility.hpp"

namespace plnt {
	template <typename T>
	struct vector_3d;

	// 二次元ベクトルテンプレート
	template <typename T>
	struct vector_2d {
		// 要素
		T x, y;

		// コンストラクタ
		constexpr vector_2d(): x(0), y(0) {}
		constexpr vector_2d(T m_x, T m_y): x(m_x), y(m_y) {}
		constexpr vector_2d(const vector_2d &obj) = default;
		constexpr vector_2d(vector_2d &&obj) = default;

		template <typename T2>
		explicit constexpr vector_2d(const vector_2d<T2> &obj): x(static_cast<T>(obj.x)), y(static_cast<T>(obj.y)) {}

		explicit constexpr vector_2d(const vector_3d<T> &v3): x(v3.x), y(v3.y) {}

		constexpr ~vector_2d() = default;

		// 代入演算子
		constexpr vector_2d &operator=(const vector_2d &obj) = default;

		// ムーブ演算子
		constexpr vector_2d &operator=(vector_2d &&obj) = default;

		// キャスト演算子
		template <typename T2>
		explicit constexpr operator vector_2d<T2>() const {
			return vector_2d<T2>(static_cast<T2>(x), static_cast<T2>(y));
		}

		// 加減算演算子
		constexpr vector_2d operator+(const vector_2d &in) const { return vector_2d(x + in.x, y + in.y); }

		constexpr vector_2d operator-(const vector_2d &in) const { return vector_2d(x - in.x, y - in.y); }

		// スカラーとの乗除算演算子
		template <typename U>
		constexpr vector_2d operator*(U num) const {
			return vector_2d(static_cast<T>(x * num), static_cast<T>(y * num));
		}

		template <typename U>
		constexpr vector_2d operator/(U num) const {
			if (num == 0) { throw std::range_error("Divided by zero."); }
			vector_2d out(static_cast<T>(static_cast<double>(x) / num),
			              static_cast<T>(static_cast<double>(y) / num));
			return out;
		}

		// 加減代入演算子
		constexpr vector_2d &operator+=(const vector_2d &in) {
			x += in.x;
			y += in.y;
			return *this;
		}

		constexpr vector_2d &operator-=(const vector_2d &in) {
			x -= in.x;
			y -= in.y;
			return *this;
		}

		// スカラーとの乗除算代入演算子
		template <typename U>
		constexpr vector_2d operator*=(U num) {
			x *= num;
			y *= num;
			return *this;
		}

		template <typename U>
		constexpr vector_2d operator/=(U num) {
			if (num == 0) { throw std::range_error("Divided by zero."); }
			x /= num;
			y /= num;
			return *this;
		}

		// 符号反転演算子
		constexpr vector_2d operator-() const { return vector_2d(-x, -y); }

		// 比較演算子（!=は自動導出）
		constexpr bool operator==(const vector_2d &in) const = default;

		// その他関数
		constexpr void set(T mx, T my) {
			x = mx;
			y = my;
		}

		// 平行な単位ベクトルを求める(同じ向きのもの)
		[[nodiscard]] constexpr vector_2d get_parallel_unit_vector() const {
			const double v_length(length());
			// NOLINTNEXTLINE(clang-diagnostic-float-equal)
			if (v_length == 0) { throw std::range_error("tried to calc unit vector from zero vector"); }
			return operator/(v_length);
		}

		//垂直単位ベクトルを求める(時計回りに90度回転したもの)
		[[nodiscard]] constexpr vector_2d get_vertical_unit_vector() const {
			return vector_2d(-y, x).get_parallel_unit_vector();
		}

		// v1の四捨五入値ベクトルを取得
		[[nodiscard]] constexpr vector_2d get_round_vector() const {
			return vector_2d(static_cast<T>(round(static_cast<double>(x))),
			                 static_cast<T>(round(static_cast<double>(y))));
		}

		[[nodiscard]] constexpr double length() const { return sqrt(length_square()); }

		[[nodiscard]] constexpr double length_square() const { return x * x + y * y; }

		[[nodiscard]] std::string to_string() const {
			using namespace std;
			stringstream is;
			is << "(" << x << "," << y << ")";
			return is.str();
		}

		// 定義済みベクトル
		static const vector_2d zero_vector;
		static const vector_2d x_unit_vector;
		static const vector_2d y_unit_vector;
	};

	template <typename T>
	const vector_2d<T> vector_2d<T>::zero_vector(0, 0);
	template <typename T>
	const vector_2d<T> vector_2d<T>::x_unit_vector(1, 0);
	template <typename T>
	const vector_2d<T> vector_2d<T>::y_unit_vector(0, 1);

	// 二次元ベクトル関数
	// 内積を求める
	template <typename T>
	[[nodiscard]] constexpr auto dot(const vector_2d<T> &v1, const vector_2d<T> &v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	// 外積を求める
	template <typename T>
	[[nodiscard]] constexpr auto cross(const vector_2d<T> &v1, const vector_2d<T> &v2) {
		return v1.x * v2.y - v1.y * v2.x;
	}

	// v2がv1となす角のコサイン値を求める(時計周りが正)
	template <typename T>
	[[nodiscard]] double get_cos(const vector_2d<T> &v1, const vector_2d<T> &v2) {
		const double v1_length(v1.length()), v2_length(v2.length());
		const double v1_v2_dot(dot(v1, v2));
		return v1_v2_dot / (v1_length * v2_length);
	}

	// v2がv1となす角のサイン値を求める(時計周りが正)
	template <typename T>
	[[nodiscard]] double get_sin(const vector_2d<T> &v1, const vector_2d<T> &v2) {
		const double v1_length(v1.length());
		const double v2_length(v2.length());
		const double v1_v2_cross(cross(v1, v2));
		return v1_v2_cross / (v1_length * v2_length);
	}

	// v2がv1となす角の角度(ラジアン)を求める(時計周りが正)
	template <typename T>
	[[nodiscard]] double get_radian(const vector_2d<T> &v1, const vector_2d<T> &v2) {
		const double v1_v2_cos(get_cos(v1, v2));
		const double v1_v1_sin(get_sin(v1, v2));
		double v1_v2_rad(acos(v1_v2_cos));
		if (v1_v1_sin < 0) { v1_v2_rad = -v1_v2_rad; }
		return v1_v2_rad;
	}

	// v2がv1となす角の角度(度)を求める(時計周りが正)
	template <typename T>
	[[nodiscard]] double get_degree(const vector_2d<T> &v1, const vector_2d<T> &v2) {
		const double v1_v2_rad = (get_radian(v1, v2));
		const double v1_v2_deg(v1_v2_rad / math::pi * 180);
		return v1_v2_deg;
	}

	// v1のv2に対する正射影ベクトルを求める
	template <typename T>
	[[nodiscard]] vector_2d<T> get_orthogonal_projection_vector(const vector_2d<T> &v1,
	                                                            const vector_2d<T> &v2) {
		return v2.get_parallel_unit_vector() * vector_2d<T>::dot(v1, v2);
	}

	// 指定した角度方向の単位ベクトルを取得する
	template <typename T>
	[[nodiscard]] vector_2d<T> get_unit_vector_by_radian(const double rad) {
		return vector_2d<T>(static_cast<T>(std::cos(rad)),
		                    static_cast<T>(std::sin(rad)));
	}

	// 二次元ベクトル型定義
	using vector_2df = vector_2d<float>; // 単精度浮動小数点型二次元ベクトル
	using vector_2dd = vector_2d<double>; // 倍精度浮動小数点型二次元ベクトル
	using vector_2di = vector_2d<int32_t>; // 32bit符号付き整数型二次元ベクトル

	// ReflectionシステムのPtree読み込みを有効にするための定義
	template <typename T>
	struct reflection::reflective_ptree_converter_impl<vector_2d<T>> {
		void operator()(vector_2d<T> &dst, const boost::property_tree::ptree &src) {
			if (src.size() != 2) {
				throw reflection_error(util::convert_and_connect_to_string(
					"要素数が", src.size(),
					"ですが、Vector2Dでは2である必要があります。"));
			}
			size_t idx = 0;
			std::array<T, 2> ary;
			for (const auto &[key, value] : src) {
				if (key.empty() == false) {
					throw reflection_error(util::convert_and_connect_to_string(
						"Vector2DのPtreeキーは空である必要があります。(読み取られたキー:",
						key, ")"));
				}
				T dat{};
				reflective_ptree_converter(dat, value);
				ary[idx++] = std::move(dat);
			}
			dst.set(ary[0], ary[1]);
		}
	};
}
