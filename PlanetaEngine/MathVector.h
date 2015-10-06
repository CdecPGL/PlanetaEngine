#pragma once
#include <array>
#include "Matrix.h"

namespace planeta_engine {
	namespace math {
		template<typename T,int S>
		class Vector {
			static_assert(S > 0, "Invalid Vector size. Vector size must be bigger than 0");
		public:
			Vector() { elements_.fill(0); }
			Vector(const Vector<T, S>& v) :elements_(v.elements_) {}
			Vector(Vector<T, S>&& v) :elements_(std::move(v.elements_)) {}
			explicit Vector(std::initializer_list<T> l) {
				for (size_t i = 0; i < l.size() && i < S; ++i) {
					elements_[i] = l[i];
				}
			}
			Vector<T, S>& operator=(const Vector<T, S>& v) { elements_ = v.elements_; return *this; }
			Vector<T, S>& operator=(Vector<T, S>&& v) { elements_ = std::move(v.elements_); return *this; }
			T& operator(size_t idx) { return elements_[idx]; }
			const T& operator(size_t idx)const { return elements_[idx]; }
			//çsóÒÇ∆ÇÃèÊéZ
			template<typename TM,int X>
			auto operator*(const Matrix<TM, X, S>& m) -> Vector<decltype(T()*TM()), X> {
				Vector<decltype(T()*TM()), X> out;
				for (int x = 0; x < X; ++x) {
					for (int y = 0; y < S; ++y) {
						out[x] += elements_[y] * m[x][y];
					}
				}
				return std::move(out);
			}
		private:
			std::array<T,S> elements_;
		};
		template<typename T>
		using Vector2D = Vector<T, 2>;
	}
}