#pragma once

#include "DxLib.h"

#include "..\core\vector_2d.hpp"

namespace plnt {
	class color;

	namespace dxlib {
		COLOR_U8 pe_color_to_dx_color_u8(const color &color);
		unsigned int pe_color_to_dx_color_handle(const color &color);

		template <typename T>
		VECTOR pe_vector_2d_to_dx_vector(const vector_2d<T> &v) {
			VECTOR dxv;
			dxv.x = static_cast<float>(v.x);
			dxv.y = static_cast<float>(v.y);
			dxv.z = 0.0f;
			return dxv;
		}
	}
}
