#pragma once
#include "DxLib.hpp"
#include "Vector2D.hpp"

namespace planeta {
	class Color;
	namespace util {
		namespace dx {
			COLOR_U8 PEColorToDXCOLORU8(const planeta::Color& color);
			unsigned int PEColorToDXColorHandle(const planeta::Color& color);
			template<typename T>
			VECTOR PEVector2DToDXVECTOR(const Vector2D<T>& v) {
				VECTOR dxv;
				dxv.x = (float)v.x;
				dxv.y = (float)v.y;
				dxv.z = 0.0f;
				return dxv;
			}
		}
	}
}
