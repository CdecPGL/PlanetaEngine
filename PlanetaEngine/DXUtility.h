#pragma once
#include "DxLib.h"
#include "Vector2D.h"

namespace planeta_engine {
	class Color;
	namespace utility {
		namespace dx {
			COLOR_U8 PEColorToDXCOLORU8(const planeta_engine::Color& color);
			unsigned int PEColorToDXColorHandle(const planeta_engine::Color& color);
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
