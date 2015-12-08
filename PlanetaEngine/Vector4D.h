#pragma once

#include <cstdint>
#include "MathVector.h"

namespace planeta_engine {
	namespace math {
		namespace vei {
			template<typename T>
			class XYZWIdentifer {
			public:
				T x()const { return ptr_[0]; }
				void x(T v) { ptr_[0] = v; }
				T y()const { return ptr_[1]; }
				void y(T v) { ptr_[1] = v; }
				T z()const { return ptr_[2]; }
				void z(T v) { ptr_[2] = v; }
				T w()const { return ptr_[3]; }
				void w(T v) { ptr_[3] = v; }
			protected:
				static constexpr int minimum_vector_size = 4;
				void SetElementArrayPointer(T* ptr) { ptr_ = ptr; }
			private:
				T* ptr_;
			};
		}
		template<typename T>
		using Vector4D = Vector<T, 4, vei::XYZWIdentifer>; //�l�����x�N�g��
		using Vector4Df = Vector4D<float>; //�P���x���������_�^�l�����x�N�g��
		using Vector4Dd = Vector4D<double>; //�{���x���������_�^�l�����x�N�g��
		using Vector4Di = Vector4D<int32_t>; //32bit�����t�������^�l�����x�N�g��
	}
}
