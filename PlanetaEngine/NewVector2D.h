#pragma once

#include <cstdint>
#include "MathVector.h"
#include "Property.h"

namespace planeta_engine {
	namespace system {
		namespace vei {
			template<typename T>
			class XYIdentifer {
				using MyType = XYIdentifer<T>;
			protected:
				static constexpr int minimum_vector_size = 2;
				void SetElementArrayPointer(T* ptr) { ptr_ = ptr; }
			private:
				T* ptr_;
				T get_x()const { return ptr_[0]; }
				void set_x(T v) { ptr_[0] = v; }
				T get_y()const { return ptr_[1]; }
				void set_y(T v) { ptr_[1] = v; }
			public:
				XYIdentifer() :x(*this), y(*this) {}
				utility::PropertyV<MyType, T, &MyType::get_x, &MyType::set_x> x;
				utility::PropertyV<MyType, T, &MyType::get_y, &MyType::set_y> y;
			};
		}
	}
	template<typename T>
	using Vector2D = math::Vector<T, 2, system::vei::XYIdentifer>; //�񎟌��x�N�g��
	using Vector2Df = Vector2D<float>; //�P���x���������_�^�񎟌��x�N�g��
	using Vector2Dd = Vector2D<double>; //�{���x���������_�^�񎟌��x�N�g��
	using Vector2Di = Vector2D<int32_t>; //32bit�����t�������^�񎟌��x�N�g��

}
