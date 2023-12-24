#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <stdexcept>
#include <cmath>
#include "planeta/math/MathConstant.hpp"
#include "planeta/reflection/ReflectionExceptions.hpp"
#include "planeta/core/StringUtility.hpp"

#define PE_ENABLE_REFLECTION_SYSTEM

namespace plnt {
	template<typename T>
	struct Vector3D;
	//二次元ベクトルテンプレート
	template<typename T> struct Vector2D {
	public:
		//要素
		T x, y;
		//コンストラクタ
		constexpr Vector2D() :x(0), y(0) {};
		constexpr Vector2D(T m_x, T m_y) : x(m_x), y(m_y) {};
		constexpr Vector2D(const Vector2D<T>& obj) :x(obj.x), y(obj.y) {}
		template<typename T2>
		explicit constexpr Vector2D(const Vector2D<T2>& obj) : x(static_cast<T>(obj.x)), y(static_cast<T>(obj.y)) {}
		constexpr Vector2D(const Vector3D<T>& v3) : x(v3.x), y(v3.y) {}
		//代入演算子
		Vector2D<T>& operator =(const Vector2D<T>& obj) { x = obj.x; y = obj.y; return *this; }
		//キャスト演算子
		template<typename T2>
		explicit constexpr operator Vector2D<T2>()const { return Vector2D<T2>(static_cast<T2>(x), static_cast<T2>(y)); }
		//加減算演算子
		constexpr Vector2D<T> operator+(const Vector2D<T>& in)const {
			return Vector2D<T>(x + in.x, y + in.y);
		}
		constexpr Vector2D<T>operator-(const Vector2D<T>& in)const {
			return Vector2D<T>(x - in.x, y - in.y);
		}
		//スカラーとの乗除算演算子
		template<typename U>
		constexpr Vector2D<T> operator*(U num)const {
			return Vector2D<T>((T)(x*num), (T)(y*num));
		}
		template<typename U>
		Vector2D<T> operator/(U num)const {
			if (num == 0) {
				throw std::range_error("Divided by zero.");
			}
			Vector2D<T> out(static_cast<T>(static_cast<double>(x) / num), static_cast<T>(static_cast<double>(y) / num));
			return out;
		}
		//加減代入演算子
		Vector2D<T>& operator+=(const Vector2D<T>& in) {
			x += in.x;
			y += in.y;
			return *this;
		}
		Vector2D<T>& operator-=(const Vector2D<T>& in) {
			x -= in.x;
			y -= in.y;
			return *this;
		}
		//スカラーとの乗除算代入演算子
		template<typename U>
		Vector2D<T> operator*=(U num) {
			x *= num;
			y *= num;
			return *this;
		}
		template<typename U>
		Vector2D<T> operator/=(U num) {
			if (num == 0) {
				throw std::range_error("Divided by zero.");
			}
			x /= num;
			y /= num;
			return *this;
		}
		//符号反転演算子
		constexpr Vector2D<T> operator-()const {
			return  Vector2D<T>(-x, -y);
		}
		//比較演算子
		constexpr bool operator==(const Vector2D<T>& in)const {
			return in.x == x && in.y == y;
		}
		constexpr bool operator!=(const Vector2D<T>& in)const {
			return !(*this == in);
		}
		//その他関数
		void Set(T mx, T my) {
			x = mx; y = my;
		}
		double length()const {
			return sqrt(length_square());
		}
		constexpr double length_square()const {
			return x*x + y*y;
		}
		const std::string ToString()const {
			using namespace std;
			stringstream is;
			is << "(" << x << "," << y << ")";
			return is.str();
		}

		//定義済みベクトル
		static const Vector2D<T> zero_vector;
		static const Vector2D<T> x_unit_vector;
		static const Vector2D<T> y_unit_vector;
	};

	template<typename T>
	const Vector2D<T> Vector2D<T>::zero_vector(0, 0);
	template<typename T>
	const Vector2D<T> Vector2D<T>::x_unit_vector(1, 0);
	template<typename T>
	const Vector2D<T> Vector2D<T>::y_unit_vector(0, 1);

	//二次元ベクトル関数
	//内積を求める
	template<typename T>
	constexpr auto Dot(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		return v1.x*v2.x + v1.y*v2.y;
	}
	//外積を求める
	template<typename T>
	constexpr auto Cross(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		return v1.x*v2.y - v1.y*v2.x;
	}
	//平行な単位ベクトルを求める(同じ向きのもの)
	template<typename T>
	Vector2D<T> GetParallelUnitVector(const Vector2D<T>& v) {
		double v_length(v.length());
		if (v_length == 0) { throw std::range_error("tryed to calc unit vector from zero vector"); }
		Vector2D<T>out(v / v.length());
		return out;
	}
	//垂直単位ベクトルを求める(時計回りに90度回転したもの)
	template<typename T>
	Vector2D<T> GetVerticalUnitVector(const Vector2D<T>& v) {
		Vector2D<T> v_vtcl(-v.y, v.x);
		double v_length(v_vtcl.length());
		Vector2D<T> out(GetParallelUnitVector(v_vtcl));
		return out;
	}
	//v2がv1となす角のコサイン値を求める(時計周りが正)
	template<typename T>
	double GetCosin(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		double v1_length(v1.length()), v2_length(v2.length());
		double v1v2_dot(Dot(v1, v2));
		return v1v2_dot / (v1_length*v2_length);
	}
	//v2がv1となす角のサイン値を求める(時計周りが正)
	template<typename T>
	double GetSin(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		double v1_length(v1.length()), v2_length(v2.length());
		double v1v2_cross(Cross(v1, v2));
		return v1v2_cross / (v1_length*v2_length);
	}
	//v2がv1となす角の角度(ラジアン)を求める(時計周りが正)
	template<typename T>
	double GetRadian(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		double v1v2_cos(GetCosin(v1, v2));
		double v1v1_sin(GetSin(v1, v2));
		double v1v2_rad(acos(v1v2_cos));
		if (v1v1_sin < 0) { v1v2_rad = -v1v2_rad; }
		return v1v2_rad;
	}
	//v2がv1となす角の角度(度)を求める(時計周りが正)
	template<typename T>
	double GetDegree(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		double v1v2_rad = (GetRadian(v1, v2));
		double v1v2_deg(v1v2_rad / math::PI * 180);
		return v1v2_deg;
	}
	//v1の四捨五入値ベクトルを取得
	template<typename T>
	Vector2D<T> GetRoundVector(const Vector2D<T>& v) {
		return Vector2D<T>((T)round((double)v.x), (T)round((double)v.y));
	}
	//v1のv2に対する正射影ベクトルを求める
	template<typename T>
	Vector2D<T> GetOrthogonalProjectionVector(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		return GetParallelUnitVector(v2) * Vector2D<T>::Dot(v1, v2);
	}
	//指定した角度方向の単位ベクトルを取得する
	template<typename T>
	Vector2D<T> GetUnitVectorByRadian(double rad) {
		return Vector2D<T>(static_cast<T>(std::cos(rad)), static_cast<T>(std::sin(rad)));
	}

	//二次元ベクトル型定義
	using Vector2Df = Vector2D<float>; //単精度浮動小数点型二次元ベクトル
	using Vector2Dd = Vector2D<double>; //倍精度浮動小数点型二次元ベクトル
	using Vector2Di =Vector2D<int32_t>; //32bit符号付き整数型二次元ベクトル
}

#ifdef PE_ENABLE_REFLECTION_SYSTEM

#include "boost/property_tree/ptree.hpp"

namespace plnt {
	namespace reflection {
		//ReflectionシステムのPtree読み込みを有効にするための定義
		template<typename T>
		void ReflectivePtreeConverter(Vector2D<T>& dst, const boost::property_tree::ptree& src) {
			if (src.size() != 2) {
				throw reflection_error(::plnt::util::ConvertAndConnectToString("要素数が", src.size(), "ですが、Vector2Dでは2である必要があります。"));
			}
			size_t idx = 0;
			std::array<T, 2> ary;
			for (auto&& pp : src) {
				if (pp.first.empty() == false) {
					throw plnt::reflection::reflection_error(::plnt::util::ConvertAndConnectToString("Vector2DのPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")")); \
				}
				T dat{};
				ReflectivePtreeConverter(dat, pp.second);
				ary[idx++] = std::move(dat);
			}
			dst.Set(ary[0], ary[1]);
		}
	}
}

#endif