#pragma once

#include <cstdint>
#include "MathVector.h"

namespace planeta_engine {
	namespace math {
		namespace vei {
			template<typename T>
			class XYZIdentifer {
			public:
				T x()const { return ptr_[0]; }
				void x(T v) { ptr_[0] = v; }
				T y()const { return ptr_[1]; }
				void y(T v) { ptr_[1] = v; }
				T z()const { return ptr_[2]; }
				void z(T v) { ptr_[2] = v; }
			protected:
				static constexpr int minimum_vector_size = 3;
				void SetElementArrayPointer(T* ptr) { ptr_ = ptr; }
			private:
				T* ptr_;
			};
		}
		template<typename T>
		using Vector3D = Vector<T,3,vei::XYZIdentifer>; //�O�����x�N�g��
		using Vector3Df = Vector3D<float>; //�P���x���������_�^�O�����x�N�g��
		using Vector3Dd = Vector3D<double>; //�{���x���������_�^�O�����x�N�g��
		using Vector3Di = Vector3D<int32_t>; //32bit�����t�������^�O�����x�N�g��
	}
}