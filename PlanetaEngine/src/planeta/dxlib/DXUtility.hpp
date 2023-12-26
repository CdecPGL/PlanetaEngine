#pragma once

#include "DxLib.h"

#include "planeta/core/Vector2D.hpp"

namespace plnt {
	class Color;

	namespace dxlib {
		COLOR_U8 PEColorToDXCOLORU8(const plnt::Color &color);
		unsigned int PEColorToDXColorHandle(const plnt::Color &color);

		template <typename T>
		VECTOR PEVector2DToDXVECTOR(const Vector2D<T> &v) {
			VECTOR dxv;
			dxv.x = (float)v.x;
			dxv.y = (float)v.y;
			dxv.z = 0.0f;
			return dxv;
		}
	}
}
