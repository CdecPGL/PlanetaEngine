#pragma once

#include"Vector2D.h"
#include <array>
#include<initializer_list>

namespace planeta_engine{
	namespace math{
		template < typename V, int X, int Y >
		class Matrix{
			/*縦横が正か確認*/
			static_assert(X > 0 && Y > 0, "Invalid Matrix size. Matrix size must be bigger than 0");
		public:
			/*デフォルトコンストラクタ(全ての要素を0にセットする)*/
			Matrix() {
				for (int i = 0; i < X; ++i) {
					elements_[i].fill(0);
				}
			}
			/*コピーコンストラクタ*/
			Matrix(const Matrix<V, X, Y>& m) :elements_(m.elements_) {}
			Matrix(Matrix<V, X, Y>&& m) :elements_(std::move(m.elements_)) {}
			explicit Matrix(std::initializer_list<std::array<V,X>> l) {
				for (size_t i = 0; i < l.size() && i < Y; ++i) {
					for (int j = 0; j < X; ++j) {
						elements_[j][i] = l[i][j];
					}
				}
			}
			/*代入演算子*/
			Matrix<V, X, Y>& operator=(const Matrix<V, X, Y>& m) { elements_ = m.elements_; return *this; }
			/*ムーブ演算子*/
			Matrix<V, X, Y>& operator=(Matrix<V, X, Y>&& m) { elements_ = std::move(m.elements_); return *this; }
			/*要素にアクセス*/
			std::array<V, Y>& operator[](size_t idx) { return elements_[idx]; }
			const std::array<V, Y>& operator[](size_t idx)const { return elements_[idx]; }
			//行列との乗算
			template<typename TM,int X2>
			auto operator*(const Matrix<TM, X2, Y>& m) ->Matrix<decltype(V()*TM()), X2, Y> {
				Matrix<decltype(std::declval<V>()*std::declval<TM>()), X2, Y> out;
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
			auto operator*(const Vector<TV,X>& m) {
				Vector<decltype(std::declval<V>()*std::declval<TV>()), X> out;
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
		auto operator*(const Vector<VT, Y>& m) {
			Vector<decltype(std::declval<VT>()*std::declval<MT>()), Y> out;
			for (int x = 0; x < X; ++x) {
				for (int y = 0; y < Y; ++y) {
					out[x] += elements_[y] * m[x][y];
				}
			}
			return std::move(out);
		}

		template<typename T>
		Vector2D<T> LinearTransformation(const Matrix<T, 2, 2>& m, const Vector2D<T>& v){
			return Vector2D<T>(m[0][0] * v.x + m[1][0] * v.y, m[0][1] * v.x + m[1][1] * v.y);
		}

		template<typename T>
		Vector2D<T> RotationalTransformation(double rota, const Vector2D<T>& v){
			Matrix<T, 2, 2> m;
			m[0][0] = (T)cos(rota);
			m[1][0] = -(T)sin(rota);
			m[0][1] = (T)sin(rota);
			m[1][1] = (T)cos(rota);
			return LinearTransformation(m, v);
		}
	}
}
