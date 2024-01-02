#pragma once

#include <array>
#include <cassert>
#include <type_traits>

#include "math_vector_element_identifier.hpp"

#include "..\core\vector_2d.hpp"

namespace plnt::math {
	/*ベクトルクラス<要素の型,要素数,要素識別子付与クラス>*/
	template <typename T, int S, template<typename>class VectorElementIdentifier = vei::dummy_identifier>
	class math_vector : public VectorElementIdentifier<T> {
		/*サイズが0より大きいか確認*/
		static_assert(S > 0, "Invalid Vector size. The vector size must be bigger than 0");
		/*型が算術型か確認*/
		static_assert(std::is_arithmetic_v<T> == true, "The element type must to be an arithmetic type.");
		/*自分の型*/
		using my_type = math_vector;
		/*要素識別子付与クラス型*/
		using vector_element_identifier_type = VectorElementIdentifier<T>;
		/*自分の要素識別子付与クラス型を持つVector型*/
		template <typename ET, int VS>
		using my_vei_vector_type = math_vector<ET, VS, VectorElementIdentifier>;
		/*要素識別子付与クラスの最低サイズを満たしているか確認*/
		static_assert(S >= vector_element_identifier_type::minimum_vector_size,
		              "The vector size must not to be less than VectorElementIdentifier::minimum_vector_size.");

	public:
		/*デフォルトコンストラクタ。要素を全て0にする*/
		math_vector() {
			SetElementArrayPointer(elements_.data());
			elements_.fill(0);
		}

		~math_vector() = default;

		/*コピーコンストラクタ*/
		math_vector(const my_type &v) : elements_(v.elements_) { SetElementArrayPointer(elements_.data()); }
		/*ムーブコンストラクタ(std::arrayで要素を持つので無意味)*/
		math_vector(my_type &&v) noexcept : elements_(std::move(v.elements_)) {
			SetElementArrayPointer(elements_.data());
		}

		/*可変長の値をとるコンストラクタ(暗黙的変換を拒否すると初期化リストで初期化できない)*/
		template <typename... ArgT>
		explicit math_vector(const T &first_arg, ArgT... args) : elements_({first_arg, args...}) {
			static_assert(S == (sizeof...(ArgT) + 1),
			              "The number of constructor argument must equal to Vector size.");
			SetElementArrayPointer(elements_.data());
		}

		/*サイズや型の異なるベクトルで初期化(暗黙的変換を許す)*/
		template <typename T2, int S2, template<typename>class VEI>
		explicit math_vector(const math_vector<T2, S2, VEI> &v) {
			SetElementArrayPointer(elements_.data());
			for (int i = 0; i < S; ++i) { (*this)[i] = i < S2 ? v[i] : 0; }
		}

		//代入演算子
		my_type &operator=(const my_type &v) {
			elements_ = v.elements_;
			return *this;
		}

		//ムーブ演算子
		my_type &operator=(my_type &&v) noexcept {
			elements_ = std::move(v.elements_);
			return *this;
		}

		/*サイズや型の異なるベクトルを代入*/
		template <typename T2, int S2, template<typename>class VEI>
		my_type &operator=(const math_vector<T2, S2, VEI> &v) {
			for (int i = 0; i < S; ++i) { (*this)[i] = static_cast<T>(i < S2 ? v[i] : 0); }
			return *this;
		}

		//等号演算子(サイズが異なる場合はかぶる部分のみで比較)
		template <typename T2, int S2, template<typename>class VEI>
		bool operator==(const math_vector<T2, S2, VEI> &v) const {
			constexpr int min_s = S < S2 ? S : S2;
			for (int i = 0; i < min_s; ++i) { if ((*this)[i] != v[i]) { return false; } }
			return true;
		}

		template <typename T2, int S2, template<typename>class VEI>
		bool operator!=(const math_vector<T2, S2, VEI> &v) const { return !(*this == v); }

		//要素にアクセス
		T &operator[](size_t idx) { return elements_[idx]; }
		const T &operator[](size_t idx) const { return elements_[idx]; }
		//ベクトルとの加算
		template <typename T2, int S2, template<typename>class VEI>
		const math_vector<T, S> &operator+=(const math_vector<T2, S2, VEI> &v) {
			constexpr int min_s = S < S2 ? S : S2;
			for (int i = 0; i < min_s; ++i) { (*this)[i] += v[i]; }
			return *this;
		}

		template <typename T2, int S2, template<typename>class VEI>
		auto operator+(const math_vector<T2, S2, VEI> &v) const {
			constexpr int max_s = S < S2 ? S2 : S;
			my_vei_vector_type<decltype(std::declval<T>() + std::declval<T2>()), max_s> ret;
			constexpr int min_s = S < S2 ? S : S2;
			for (int i = 0; i < min_s; ++i) { ret[i] += v[i]; }
			return std::move(ret);
		}

		//ベクトルとの減算
		template <typename T2, int S2, template<typename>class VEI>
		const my_type &operator-=(const math_vector<T2, S2, VEI> &v) {
			constexpr int min_s = S < S2 ? S : S2;
			for (int i = 0; i < min_s; ++i) { (*this)[i] -= v[i]; }
			return *this;
		}

		template <typename T2, int S2, template<typename>class VEI>
		auto operator-(const math_vector<T2, S2, VEI> &v) const {
			constexpr int max_s = S < S2 ? S2 : S;
			my_vei_vector_type<decltype(std::declval<T>() - std::declval<T2>()), max_s> ret;
			constexpr int min_s = S < S2 ? S : S2;
			for (int i = 0; i < min_s; ++i) { ret[i] -= v[i]; }
			return std::move(ret);
		}

		//スカラーとの乗算
		template <typename T2>
		const math_vector<T, S> &operator*=(const T2 &v) {
			for (int i = 0; i < S; ++i) { (*this)[i] *= v; }
			return *this;
		}

		template <typename T2>
		auto operator*(const T2 &v) const {
			my_vei_vector_type<decltype(std::declval<T>() * std::declval<T2>()), S> ret;
			for (int i = 0; i < S; ++i) { ret[i] *= v; }
			return std::move(ret);
		}

		//スカラーとの除算
		template <typename T2>
		const my_type &operator/=(const T2 &v) {
			assert(v != 0);
			for (int i = 0; i < S; ++i) { (*this)[i] /= v; }
			return *this;
		}

		template <typename T2>
		auto operator/(const T2 &v) const {
			assert(v != 0);
			my_vei_vector_type<decltype(std::declval<T>() / std::declval<T2>()), S> ret;
			for (int i = 0; i < S; ++i) { ret[i] /= v; }
			return std::move(ret);
		}

		/*単項演算子-*/
		my_type operator-() const {
			my_type ret = *this;
			for (int i = 0; i < S; ++i) { ret[i] = -ret[i]; }
			return std::move(ret);
		}

		/*長さの二乗*/
		T length_square() const {
			T ret = 0;
			for (int i = 0; i < S; ++i) { ret += (*this)[i] * (*this)[i]; }
			return ret;
		}

		/*長さ*/
		auto length() const { return std::sqrt(length_square()); }
		/*要素のセット*/
		template <typename... ArgT>
		void set(ArgT... args) {
			static_assert(S == (sizeof...(ArgT)), "The number of set function argument must equal to Vector size.");
			elements_ = {args...};
		}

	private:
		std::array<T, S> elements_;
	};

	//組み込み型との演算子
	template <typename ScalaT, typename VectorT, int VectorS, template<typename>class VEI>
	auto operator*(const ScalaT &s, const math_vector<VectorT, VectorS, VEI> &v) { return v * s; }
}
