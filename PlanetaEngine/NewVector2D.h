#pragma once

#include <cstdint>
#include "MathVector.h"
#include "Property.h"

//旧Vector2Dとの競合回避ため、ファイル名とクラス名をNewVector2Dにしておく。
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
				utility::ReadWritePropertyV<MyType, T, &MyType::get_x, &MyType::set_x> x;
				utility::ReadWritePropertyV<MyType, T, &MyType::get_y, &MyType::set_y> y;
			};
		}
	}
	template<typename T>
	using NewVector2D = math::Vector<T, 2, system::vei::XYIdentifer>; //二次元ベクトル
	using Vector2Df = NewVector2D<float>; //単精度浮動小数点型二次元ベクトル
	using Vector2Dd = NewVector2D<double>; //倍精度浮動小数点型二次元ベクトル
	using Vector2Di = NewVector2D<int32_t>; //32bit符号付き整数型二次元ベクトル

}
