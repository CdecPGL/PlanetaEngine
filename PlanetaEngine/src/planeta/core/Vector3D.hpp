#pragma once

#include <cstdint>
#include <string>
#include "..\math\math_constant.hpp"

namespace plnt {
	template <typename T>
	struct Vector2D;

	//三次元ベクトルテンプレート
	template <typename T>
	struct Vector3D {
	public:
		//要素
		T x, y, z;
		//コンストラクタ
		constexpr Vector3D() : x(0), y(0), z(0) { };

		constexpr Vector3D(T m_x, T m_y, T m_z) : x(m_x), y(m_y), z(m_z) { };

		constexpr Vector3D(const Vector3D<T> &obj) : x(obj.x), y(obj.y), z(obj.z) { }

		template <typename T2>
		explicit constexpr Vector3D(const Vector3D<T2> &obj) : x(static_cast<T>(obj.x)), y(static_cast<T>(obj.y)),
		                                                       z(static_cast<T>(obj.z)) { }

		constexpr Vector3D(const Vector2D<T> &v2) : x(v2.x), y(v2.y), z(0) { }

		//代入演算子
		Vector3D<T> &operator =(const Vector3D<T> &obj) {
			x = obj.x;
			y = obj.y;
			z = obj.z;
			return *this;
		}

		//キャスト演算子
		template <typename T2>
		explicit constexpr operator Vector3D<T2>() const {
			return Vector3D<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z));
		}

		//加減算演算子
		constexpr Vector3D<T> operator+(const Vector3D<T> &in) const {
			return Vector3D<T>(x + in.x, y + in.y, z + in.z);
		}

		constexpr Vector3D<T> operator-(const Vector3D<T> &in) const {
			return Vector3D<T>(x - in.x, y - in.y, z - in.z);
		}

		//スカラーとの乗除算演算子
		template <typename U>
		constexpr Vector3D<T> operator*(U num) const { return Vector3D<T>((T)(x * num), (T)(y * num), (T)(z * num)); }

		template <typename U>
		Vector3D<T> operator/(U num) const {
			if (num == 0) { throw std::range_error("Divided by zero."); }
			Vector3D<T> out(static_cast<T>(static_cast<double>(x) / num), static_cast<T>(static_cast<double>(y) / num),
			                static_cast<T>(static_cast<double>(z) / num));
			return out;
		}

		//加減代入演算子
		Vector3D<T> &operator+=(const Vector3D<T> &in) {
			x += in.x;
			y += in.y;
			z += in.z;
			return *this;
		}

		Vector3D<T> &operator-=(const Vector3D<T> &in) {
			x -= in.x;
			y -= in.y;
			z -= in.z;
			return *this;
		}

		//スカラーとの乗除算代入演算子
		template <typename U>
		Vector3D<T> operator*=(U num) {
			x *= num;
			y *= num;
			z *= num;
			return *this;
		}

		template <typename U>
		Vector3D<T> operator/=(U num) {
			if (num == 0) { throw std::range_error("Divided by zero."); }
			x /= num;
			y /= num;
			z /= num;
			return *this;
		}

		//符号反転演算子
		constexpr Vector3D<T> operator-() const { return Vector3D<T>(-x, -y, -z); }

		//比較演算子
		constexpr bool operator==(const Vector3D<T> &in) const { return in.x == x && in.y == y && in.z == z; }

		constexpr bool operator!=(const Vector3D<T> &in) const { return !(*this == in); }

		//その他関数
		void Set(T mx, T my, T mz) {
			x = mx;
			y = my;
			z = mz;
		}

		double length() const { return sqrt(length_square()); }

		constexpr double length_square() const { return x * x + y * y + z * z; }

		const std::string ToString() const {
			using namespace std;
			stringstream is;
			is << "(" << x << "," << y << "," << z << ")";
			return is.str();
		}

		//定義済みベクトル
		static const Vector3D<T> zero_vector;
		static const Vector3D<T> x_unit_vector;
		static const Vector3D<T> y_unit_vector;
		static const Vector3D<T> z_unit_vector;
	};

	template <typename T>
	const Vector3D<T> Vector3D<T>::zero_vector(0, 0, 0);
	template <typename T>
	const Vector3D<T> Vector3D<T>::x_unit_vector(1, 0, 0);
	template <typename T>
	const Vector3D<T> Vector3D<T>::y_unit_vector(0, 1, 0);
	template <typename T>
	const Vector3D<T> Vector3D<T>::z_unit_vector(0, 0, 1);

	//二次元ベクトル関数
	//内積を求める
	template <typename T>
	constexpr auto Dot(const Vector3D<T> &v1, const Vector3D<T> &v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

	//外積を求める
	/*template<typename T>
	constexpr auto Cross(const Vector3D<T>& v1, const Vector3D<T>& v2) {
		return v1.x*v2.y - v1.y*v2.x;
	}*/
	//平行な単位ベクトルを求める(同じ向きのもの)
	template <typename T>
	Vector3D<T> GetParallelUnitVector(const Vector3D<T> &v) {
		double v_length(v.length());
		if (v_length == 0) { throw std::range_error("tryed to calc unit vector from zero vector"); }
		Vector3D<T> out(v / v.length());
		return out;
	}

	//垂直単位ベクトルを求める(時計回りに90度回転したもの)
	/*template<typename T>
	Vector3D<T> GetVerticalUnitVector(const Vector3D<T>& v) {
		Vector3D<T> v_vtcl(-v.y, v.x);
		double v_length(v_vtcl.length());
		Vector3D<T> out(GetParallelUnitVector(v_vtcl));
		return out;
	}*/
	//v2がv1となす角のコサイン値を求める(時計周りが正)
	template <typename T>
	double GetCosin(const Vector3D<T> &v1, const Vector3D<T> &v2) {
		double v1_length(v1.length()), v2_length(v2.length());
		double v1v2_dot(Dot(v1, v2));
		return v1v2_dot / (v1_length * v2_length);
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
		double v1v2_cos(GetCosin(v1, v2));
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
	//v1の四捨五入値ベクトルを取得
	template <typename T>
	Vector3D<T> GetRoundVector(const Vector3D<T> &v) {
		return Vector3D<T>((T)round((double)v.x), (T)round((double)v.y), (T)round((double)v.z));
	}

	//v1のv2に対する正射影ベクトルを求める
	template <typename T>
	Vector3D<T> GetOrthogonalProjectionVector(const Vector3D<T> &v1, const Vector3D<T> &v2) {
		return GetParallelUnitVector(v2) * Vector3D<T>::Dot(v1, v2);
	}

	//指定した角度方向の単位ベクトルを取得する
	/*template<typename T>
	Vector3D<T> GetUnitVectorByRadian(double rad) {
		return Vector3D<T>(static_cast<T>(std::cos(rad)), static_cast<T>(std::sin(rad)));
	}*/

	using Vector3Df = Vector3D<float>; //単精度浮動小数点型三次元ベクトル
	using Vector3Dd = Vector3D<double>; //倍精度浮動小数点型三次元ベクトル
	using Vector3Di = Vector3D<int32_t>; //32bit符号付き整数型三次元ベクトル
}

#include "boost/property_tree/ptree.hpp"
#include "planeta/reflection/ReflectionUtility.hpp"

namespace plnt::reflection {
	//ReflectionシステムのPtree読み込みを有効にするための定義
	template <typename T>
	struct ReflectivePtreeConverterImpl<Vector3D<T>> {
		void operator()(Vector3D<T> &dst, const boost::property_tree::ptree &src) {
			if (src.size() != 3) {
				throw reflection_error(
					util::ConvertAndConnectToString("要素数が", src.size(), "ですが、Vector3Dでは3である必要があります。"));
			}
			size_t idx = 0;
			std::array<T, 3> ary;
			for (auto &&pp : src) {
				if (pp.first.empty() == false) {
					throw reflection_error(
						util::ConvertAndConnectToString("Vector3DのPtreeキーは空である必要があります。(読み取られたキー:", pp.first, ")"));
				}
				T dat{};
				ReflectivePtreeConverter(dat, pp.second);
				ary[idx++] = std::move(dat);
			}
			dst.Set(ary[0], ary[1], ary[2]);
		}
	};
}
