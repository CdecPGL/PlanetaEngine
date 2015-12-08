#pragma once

#include <cstdint>
#include "MathVector.h"
#include "Property.h"

namespace planeta_engine {
	namespace math {
		namespace vei {
			template<typename T>
			class XYZIdentifer {
				using MyType = XYZIdentifer<T>;
			protected:
				static constexpr int minimum_vector_size = 3;
				void SetElementArrayPointer(T* ptr) { ptr_ = ptr; }
			private:
				T* ptr_;
				T get_x()const { return ptr_[0]; }
				void set_x(T v) { ptr_[0] = v; }
				T get_y()const { return ptr_[1]; }
				void set_y(T v) { ptr_[1] = v; }
				T get_z()const { return ptr_[2]; }
				void set_z(T v) { ptr_[2] = v; }
			public:
				XYZIdentifer() :x(*this), y(*this), z(*this) {}
				utility::Property<MyType, T, &MyType::get_x, &MyType::set_x> x;
				utility::Property<MyType, T, &MyType::get_y, &MyType::set_y> y;
				utility::Property<MyType, T, &MyType::get_z, &MyType::set_z> z;
			};
		}
		template<typename T>
		using Vector3D = Vector<T,3,vei::XYZIdentifer>; //�O�����x�N�g��
		using Vector3Df = Vector3D<float>; //�P���x���������_�^�O�����x�N�g��
		using Vector3Dd = Vector3D<double>; //�{���x���������_�^�O�����x�N�g��
		using Vector3Di = Vector3D<int32_t>; //32bit�����t�������^�O�����x�N�g��
	}
}