#pragma once

#include"Vector2D.h"
#include <array>
#include<initializer_list>

namespace planeta_engine{
	namespace math{
		template < typename V, int X, int Y >
		class Matrix{
			static_assert(X > 0 && Y > 0, "Invalid Matrix size. Matrix size must be bigger than 0");
		public:
			Matrix() {
				for (int i = 0; i < X; ++i) {
					elements_[i].fill(0);
				}
			}
			Matrix(const Matrix<V, X, Y>& m) :elements_(m.elements_) {}
			Matrix(Matrix<V, X, Y>&& m) :elements_(std::move(m.elements_)) {}
			explicit Matrix(std::initializer_list<std::array<V,X>> l) {
				for (size_t i = 0; i < l.size() && i < Y; ++i) {
					for (int j = 0; j < X; ++j) {
						elements_[j][i] = l[i][j];
					}
				}
			}
			Matrix<V, X, Y>& operator=(const Matrix<V, X, Y>& m) { elements_ = m.elements_; return *this; }
			Matrix<V, X, Y>& operator=(Matrix<V, X, Y>&& m) { elements_ = std::move(m.elements_); return *this; }
			std::array<V, Y>& operator[](size_t idx) { return elements_[idx]; }
			const std::array<V, Y>& operator[](size_t idx)const { return elements_[idx]; }
			//çsóÒÇ∆ÇÃèÊéZ
			template<typename TM,int X2>
			auto operator*(const Matrix<TM, X2, Y>& m) ->Matrix<decltype(V()*TM()), X2, Y> {
				Matrix<decltype(V()*TM()), X2, Y> out;
				for (int y1 = 0; y1 < Y; ++y1) {
					for (int x = 0; x < X2; ++x) {
						for (y2 = 0; y2 < X; ++y2) {
							out[x][y1] += elements_[y2][y1] * m[x][y2];
						}
					}
				}
				return std::move(out);
			}
		private:
			std::array<std::array<V, Y>, X> elements_;
		};

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
