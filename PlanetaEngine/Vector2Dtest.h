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
		using Vector2D = Vector<T, 3,vei::XYIdentifer>; //�񎟌��x�N�g��
		using Vector2Df = Vector2D<float>; //�P���x���������_�^�񎟌��x�N�g��
		using Vector2Dd = Vector2D<double>; //�{���x���������_�^�񎟌��x�N�g��
		using Vector2Di = Vector2D<int32_t>; //32bit�����t�������^�񎟌��x�N�g��
	}
}
