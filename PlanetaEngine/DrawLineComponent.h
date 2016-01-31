#pragma once
#include "DrawComponent.h"
#include "Vector2D.h"
#include <array>
#include <vector>

namespace planeta_engine {
	namespace components {
		class DrawLineComponent final: public DrawComponent {
		public:
			DrawLineComponent();
			/*�A�N�Z�T*/
			int width()const { return _width; }
			DrawLineComponent& width(int w) { _width = w; return *this; }
			double length()const { return _length; }
			DrawLineComponent& length(double l) { _length = l; return *this; }
		private:
			std::vector<Vector2D<double>> _wire_positions;
			int _width;
			double _length;
			/*�`�揈��*/
			void DrawProc(ScreenDrawer2D& drawer)override;
		};
	}
}