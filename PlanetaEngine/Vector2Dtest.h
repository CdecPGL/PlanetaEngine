#pragma once

#include <cstdint>
#include "MathVector.h"

namespace planeta_engine {
	namespace math {
		namespace vei {
			template<typename T>
			class XYIdentifer {
			public:
				T x()const { return ptr_[0]; }
				void x(T v) { ptr_[0] = v; }
				T y()const { return ptr_[0]; }
				void y(T v) { ptr_[0] = v; }
			protected:
				static constexpr int minimum_vector_size = 2;
				void SetElementArrayPointer(T* ptr) { ptr_ = ptr; }
			private:
				T* ptr_;
			};
		}
		template<typename T>
		using Vector2D = Vector<T, 3,vei::XYIdentifer>; //二次元ベクトル
		using Vector2Df = Vector2D<float>; //単精度浮動小数点型二次元ベクトル
		using Vector2Dd = Vector2D<double>; //倍精度浮動小数点型二次元ベクトル
		using Vector2Di = Vector2D<int32_t>; //32bit符号付き整数型二次元ベクトル
	}
}
