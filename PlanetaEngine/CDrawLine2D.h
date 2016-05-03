#pragma once
#include "CDraw2D.h"
#include "Vector2D.h"
#include <array>
#include <vector>

namespace planeta_engine {
	class CDrawLine2D final : public CDraw2D {
	public:
		CDrawLine2D();
		/*�A�N�Z�T*/
		int width()const { return _width; }
		CDrawLine2D& width(int w) { _width = w; return *this; }
		double length()const { return _length; }
		CDrawLine2D& length(double l) { _length = l; return *this; }
	private:
		std::vector<Vector2Dd> _wire_positions;
		int _width;
		double _length;
		/*�`�揈��*/
		void DrawProc(ScreenDrawer2D& drawer)override;
	};
}