#include "planeta/core/color.hpp"

#include "dx_utility.hpp"

namespace plnt::dxlib {
	COLOR_U8 pe_color_to_dx_color_u8(const color &color) {
		COLOR_U8 cu8;
		cu8.r = static_cast<unsigned char>(color.r());
		cu8.g = static_cast<unsigned char>(color.g());
		cu8.b = static_cast<unsigned char>(color.b());
		cu8.a = static_cast<unsigned char>(color.a());
		return cu8;
	}

	unsigned int pe_color_to_dx_color_handle(const color &color) { return GetColor(color.r(), color.g(), color.b()); }
}
