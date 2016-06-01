#pragma once

#include <cstdint>
#include "MathVector.h"
#include "Property.h"

namespace planeta_engine {
	namespace system {
		namespace vei {
			template<typename T>
			class XYZWIdentifer {
				using MyType = XYZWIdentifer<T>;
			protected:
				static constexpr int minimum_vector_size = 4;
				void SetElementArrayPointer(T* ptr) { ptr_ = ptr; }
			private:
				T* ptr_;
				T get_x()const { return ptr_[0]; }
				void set_x(T v) { ptr_[0] = v; }
				T get_y()const { return ptr_[1]; }
				void set_y(T v) { ptr_[1] = v; }
				T get_z()const { return ptr_[2]; }
				void set_z(T v) { ptr_[2] = v; }
				T get_w()const { return ptr_[3]; }
				void set_w(T v) { ptr_[3] = v; }
			public:
				XYZWIdentifer() :x(*this), y(*this), z(*this), w(*this) {}
				util::ReadWritePropertyV<MyType, T, &MyType::get_x, &MyType::set_x> x;
				util::ReadWritePropertyV<MyType, T, &MyType::get_y, &MyType::set_y> y;
				util::ReadWritePropertyV<MyType, T, &MyType::get_z, &MyType::set_z> z;
				util::ReadWritePropertyV<MyType, T, &MyType::get_w, &MyType::set_w> w;
			};
		}
	}
	template<typename T>
	using Vector4D = math::MathVector<T, 4, system::vei::XYZWIdentifer>; //四次元ベクトル
	using Vector4Df = Vector4D<float>; //単精度浮動小数点型四次元ベクトル
	using Vector4Dd = Vector4D<double>; //倍精度浮動小数点型四次元ベクトル
	using Vector4Di = Vector4D<int32_t>; //32bit符号付き整数型四次元ベクトル
}
