#pragma once

#include <array>
#include <initializer_list>
#include <cassert>
#include <type_traits>

#include "boost/preprocessor/repeat.hpp"
#include "boost/preprocessor/if.hpp"
#include "boost/preprocessor/arithmetic/sub.hpp"

#include "Vector2D.h"

namespace planeta_engine {
	namespace math {
//座標ラベルをインデックスから取得する
#define CGS_COORDINATELABEL_GETTER(idx)\
BOOST_PP_IF(idx,BOOST_PP_IF(BOOST_PP_SUB(idx,1),BOOST_PP_IF(BOOST_PP_SUB(idx, 2),w,z),y),x)
//インデックスから座標プロパティ(ゲッタ、セッタ)を生成する
#define CGS_GETTER_SETTER_DEFINER(idx)\
T CGS_COORDINATELABEL_GETTER(idx)()const{return ptr_[idx];};\
void CGS_COORDINATELABEL_GETTER(idx)(const T& v){ptr_[idx] = v;};
//上をboost::ppで使うようラッパしたもの
#define CGS_GETTER_SETTER_DEFINER_WRAPPER(z,idx,data)\
CGS_GETTER_SETTER_DEFINER(idx)
//0-[count-1]までの座標プロパティを生成する。
#define CGS_GETTET_SETTER_DEFINER_IDX_0_TO_ARG(count)\
BOOST_PP_REPEAT(count,CGS_GETTER_SETTER_DEFINER_WRAPPER,0)
//0-[count-1]までの座標プロパティをもつインターフェイスクラスを生成する
#define CGS_DEFINER(id,dimension)\
template<typename T>\
class id {\
public:\
id(T* ptr) :ptr_(ptr) {}\
CGS_GETTET_SETTER_DEFINER_IDX_0_TO_ARG(dimension)\
private:\
T* ptr_;\
};
		namespace cgs {
			//座標プロパティクラス
			CGS_DEFINER(XGetterSetter, 1);
			CGS_DEFINER(XYGetterSetter, 2);
			CGS_DEFINER(XYZGetterSetter, 3);
			CGS_DEFINER(XYZWGetterSetter, 4);
			//次元により座標プロパティクラスを選択するエイリアステンプレート
			template<int S, typename T>
			using CoordinateGetterSetterSelector = std::conditional_t<S == 1, XGetterSetter<T>, std::conditional_t<S == 2, XYGetterSetter<T>, std::conditional_t<S == 3, XYZGetterSetter<T>, XYZWGetterSetter<T>>>>;
		}
		/*ベクトルクラス<要素の型,要素数>*/
		template<typename T,int S>
		class Vector : public cgs::CoordinateGetterSetterSelector<S,T> {
			/*サイズが0より大きいか確認*/
			static_assert(S > 0, "Invalid Vector size. Vector size must be bigger than 0");
			/*型が算術型か確認*/
			static_assert(std::is_arithmetic<T>::value == true, "The element type must to be an arithmetic type.");
			using MyType = Vector<T, S>;
			using CoordinateGetterSetter = cgs::CoordinateGetterSetterSelector<S,T>;
		public:
			/*デフォルトコンストラクタ。要素を全て0にする*/
			Vector() :CoordinateGetterSetter(elements_.data()) { elements_.fill(0); }
			/*コピーコンストラクタ*/
			Vector(const MyType& v) :CoordinateGetterSetter(elements_.data()),elements_(v.elements_) {}
			/*ムーブコンストラクタ(std::arrayで要素を持つので無意味)*/
//			Vector(MyType&& v) :elements_(std::move(v.elements_)) {}
			/*可変長の値をとるコンストラクタ(暗黙的変換を拒否すると初期化リストで初期化できない)*/
			template<typename... ArgT>
			Vector(const T& first_arg, ArgT... args) : CoordinateGetterSetter(elements_.data()), elements_({ first_arg, args... }) {
				static_assert(S == (sizeof...(ArgT)+1), "The number of constructor argument must equal to Vector size.");
			}
			/*サイズや型の異なるベクトルで初期化(暗黙的変換を許す)*/
			template<typename T2,int S2>
			Vector(const Vector<T2, S2>& v): CoordinateGetterSetter(elements_.data()) {
				for (int i = 0; i < S; ++i) {
					(*this)[i] = i < S2 ? v[i] : 0;
				}
			}
			//代入演算子
			MyType& operator=(const MyType& v) { elements_ = v.elements_; return *this; }
			//ムーブ演算子(std::arrayで要素を持つので無意味)
//			MyType& operator=(MyType&& v) { elements_ = std::move(v.elements_); return *this; }
			/*サイズや型の異なるベクトルを代入*/
			template<typename T2, int S2>
			Vector<T,S>& operator=(const Vector<T2, S2>& v) {
				for (int i = 0; i < S; ++i) {
					(*this)[i] = static_cast<T>(i < S2 ? v[i] : 0);
				}
				return *this;
			}
			//等号演算子(サイズが異なる場合はかぶる部分のみで比較)
			template<typename T2,int S2>
			bool operator==(const Vector<T2, S2>& v)const {
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) { if ((*this)[i] != v[i]) { return false; } }
				return true;
			}
			template<typename T2, int S2>
			bool operator!=(const Vector<T2, S2>& v)const {
				return !(*this == v);
			}
			//要素にアクセス
			T& operator[](size_t idx) { return elements_[idx]; }
			const T& operator[](size_t idx)const { return elements_[idx]; }
			//ベクトルとの加算
			template<typename T2,int S2>
			const Vector<T,S>& operator+=(const Vector<T2, S2>& v) {
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) {
					(*this)[i] += v[i];
				}
				return *this;
			}
			template<typename T2, int S2>
			auto operator+(const Vector<T2, S2>& v)const {
				constexpr int MaxS = S < S2 ? S2 : S;
				Vector<decltype(std::declval<T>() + std::declval<T2>()), MaxS> ret;
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) {
					ret[i] += v[i];
				}
				return std::move(ret);
			}
			//ベクトルとの減算
			template<typename T2, int S2>
			const MyType& operator-=(const Vector<T2, S2>& v) {
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) {
					(*this)[i] -= v[i];
				}
				return *this;
			}
			template<typename T2, int S2>
			auto operator-(const Vector<T2, S2>& v)const {
				constexpr int MaxS = S < S2 ? S2 : S;
				Vector<decltype(std::declval<T>() - std::declval<T2>()), MaxS> ret;
				constexpr int MinS = S < S2 ? S : S2;
				for (int i = 0; i < MinS; ++i) {
					ret[i] -= v[i];
				}
				return std::move(ret);
			}
			//スカラーとの乗算
			template<typename T2>
			const Vector<T,S>& operator*=(const T2& v) {
				for (int i = 0; i < S; ++i) { (*this)[i] *= v; }
				return *this;
			}
			template<typename T2>
			auto operator*(const T2& v)const {
				Vector<decltype(std::declval<T>()*std::declval<T2>()), S> ret;
				for (int i = 0; i < S; ++i) { ret[i] *= v; }
				return std::move(ret);
			}
			//スカラーとの除算
			template<typename T2>
			const MyType& operator/=(const T2& v) {
				assert(v != 0);
				for (int i = 0; i < S; ++i) { (*this)[i] /= v; }
				return *this;
			}
			template<typename T2>
			auto operator/(const T2& v)const {
				assert(v != 0);
				Vector<decltype(std::declval<T>()/std::declval<T2>()),S> ret;
				for (int i = 0; i < S; ++i) { ret[i] /= v; }
				return std::move(ret);
			}
			/*単項演算子-*/
			MyType operator-()const {
				MyType ret = *this;
				for (int i = 0; i < S; ++i) { ret[i] = -ret[i]; }
				return std::move(ret);
			}

			/*長さの二乗*/
			T length_square()const{
				T ret = 0;
				for (int i = 0; i < S; ++i) { ret += (*this)[i] * (*this)[i]; }
				return ret;
			}
			/*長さ*/
			auto length()const { return std::sqrt(length_square()); }
			/*要素のセット*/
			template<typename... ArgT>
			void Set(ArgT... args) {
				static_assert(S == (sizeof...(ArgT)), "The number of set function argument must equal to Vector size.");
				elements_ = { args... };
			}

			/*Vector2Dとの互換用*/
			//Vector2DからVectorへの変換(暗黙的変換を許可)
			template<typename T2>
			Vector(const ::Vector2D<T2>& v2d) : Vector(Vector<T2, 2>(v2d.x, v2d.y)) {}
			//VectorからVector2Dへの変換
			template<typename T2>
			operator ::Vector2D<T2>() {
				return ::Vector2D<T2>((*this)[0], S >= 2 ? (*this)[1] : 0);
			}

		private:
			std::array<T,S> elements_;
		};

		//組み込み型との演算子
		template<typename SCALA_T, typename VECTOR_T, int VECTOR_S>
		auto operator*(const SCALA_T& s, const Vector<VECTOR_T, VECTOR_S>& v) {
			return v*s;
		}
	}
}