#pragma once

#include <array>
#include<initializer_list>

#include"math_vector.hpp"

namespace plnt::math {
	template <typename V, int X, int Y>
	class math_matrix final {
		/*縦横が正か確認*/
		static_assert(X > 0 && Y > 0, "Invalid Matrix size. Matrix size must be bigger than 0");

	public:
		/*デフォルトコンストラクタ(全ての要素を0にセットする)*/
		math_matrix() { for (int i = 0; i < X; ++i) { elements_[i].fill(0); } }
		~math_matrix() = default;

		/*コピーコンストラクタ*/
		math_matrix(const math_matrix &m) : elements_(m.elements_) { }

		math_matrix(math_matrix &&m) noexcept : elements_(std::move(m.elements_)) { }

		explicit math_matrix(std::initializer_list<std::array<V, X>> l) {
			for (size_t i = 0; i < l.size() && i < Y; ++i) {
				for (int j = 0; j < X; ++j) { elements_[j][i] = l[i][j]; }
			}
		}

		/*代入演算子*/
		math_matrix &operator=(const math_matrix &m) {
			elements_ = m.elements_;
			return *this;
		}

		/*ムーブ演算子*/
		math_matrix &operator=(math_matrix &&m) noexcept {
			elements_ = std::move(m.elements_);
			return *this;
		}

		/*要素にアクセス*/
		std::array<V, Y> &operator[](size_t idx) { return elements_[idx]; }
		const std::array<V, Y> &operator[](size_t idx) const { return elements_[idx]; }
		//行列との乗算
		template <typename TM, int X2>
		auto operator*(const math_matrix<TM, X2, Y> &m) -> math_matrix<decltype(V() * TM()), X2, Y> {
			math_matrix<decltype(std::declval<V>() * std::declval<TM>()), X2, Y> out;
			for (int y1 = 0; y1 < Y; ++y1) {
				for (int x = 0; x < X2; ++x) {
					for (int y2 = 0; y2 < X; ++y2) { out[x][y1] += elements_[y2][y1] * m[x][y2]; }
				}
			}
			return std::move(out);
		}

		//ベクトルとの乗算
		template <typename TV>
		auto operator*(const math_vector<TV, X> &m) {
			math_vector<decltype(std::declval<V>() * std::declval<TV>()), X> out;
			for (int x = 0; x < X; ++x) { for (int y = 0; y < Y; ++y) { out[x] += elements_[y] * m[x][y]; } }
			return std::move(out);
		}

	private:
		std::array<std::array<V, Y>, X> elements_;
	};
}
