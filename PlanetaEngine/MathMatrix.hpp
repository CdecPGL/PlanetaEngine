#pragma once

#include"MathVector.h"
#include <array>
#include<initializer_list>

namespace planeta{
	namespace math{
		template < typename V, int X, int Y >
		class MathMatrix{
			/*縦横が正か確認*/
			static_assert(X > 0 && Y > 0, "Invalid Matrix size. Matrix size must be bigger than 0");
		public:
			/*デフォルトコンストラクタ(全ての要素を0にセットする)*/
			MathMatrix() {
				for (int i = 0; i < X; ++i) {
					elements_[i].fill(0);
				}
			}
			/*コピーコンストラクタ*/
			MathMatrix(const MathMatrix<V, X, Y>& m) :elements_(m.elements_) {}
			MathMatrix(MathMatrix<V, X, Y>&& m) :elements_(std::move(m.elements_)) {}
			explicit MathMatrix(std::initializer_list<std::array<V,X>> l) {
				for (size_t i = 0; i < l.size() && i < Y; ++i) {
					for (int j = 0; j < X; ++j) {
						elements_[j][i] = l[i][j];
					}
				}
			}
			/*代入演算子*/
			MathMatrix<V, X, Y>& operator=(const MathMatrix<V, X, Y>& m) { elements_ = m.elements_; return *this; }
			/*ムーブ演算子*/
			MathMatrix<V, X, Y>& operator=(MathMatrix<V, X, Y>&& m) { elements_ = std::move(m.elements_); return *this; }
			/*要素にアクセス*/
			std::array<V, Y>& operator[](size_t idx) { return elements_[idx]; }
			const std::array<V, Y>& operator[](size_t idx)const { return elements_[idx]; }
			//行列との乗算
			template<typename TM,int X2>
			auto operator*(const MathMatrix<TM, X2, Y>& m) ->MathMatrix<decltype(V()*TM()), X2, Y> {
				MathMatrix<decltype(std::declval<V>()*std::declval<TM>()), X2, Y> out;
				for (int y1 = 0; y1 < Y; ++y1) {
					for (int x = 0; x < X2; ++x) {
						for (y2 = 0; y2 < X; ++y2) {
							out[x][y1] += elements_[y2][y1] * m[x][y2];
						}
					}
				}
				return std::move(out);
			}
			//ベクトルとの乗算
			template<typename TV>
			auto operator*(const MathVector<TV,X>& m) {
				MathVector<decltype(std::declval<V>()*std::declval<TV>()), X> out;
				for (int x = 0; x < X; ++x) {
					for (int y = 0; y < Y; ++y) {
						out[x] += elements_[y] * m[x][y];
					}
				}
				return std::move(out);
			}
		private:
			std::array<std::array<V, Y>, X> elements_;
		};

		//他の型との演算子
		/*ベクトル*行列*/
		template<typename VT,typename MT,int X,int Y>
		auto operator*(const MathVector<VT, Y>& m) {
			MathVector<decltype(std::declval<VT>()*std::declval<MT>()), Y> out;
			for (int x = 0; x < X; ++x) {
				for (int y = 0; y < Y; ++y) {
					out[x] += elements_[y] * m[x][y];
				}
			}
			return std::move(out);
		}
	}
}
