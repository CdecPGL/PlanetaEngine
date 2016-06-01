#include "DXUtility.h"
#include "Color.h"

namespace planeta_engine {
	namespace util {
		namespace dx {
			COLOR_U8 PEColorToDXCOLORU8(const planeta_engine::Color& color)
			{
				COLOR_U8 cu8;
				cu8.r = (unsigned char)color.r();
				cu8.g = (unsigned char)color.g();
				cu8.b = (unsigned char)color.b();
				cu8.a = (unsigned char)color.a();
				return cu8;
			}

			unsigned int PEColorToDXColorHandle(const planeta_engine::Color& color)
			{
				return GetColor(color.r(), color.g(), color.b());
			}

		}
	}
}