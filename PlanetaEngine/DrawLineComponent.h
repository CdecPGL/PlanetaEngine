#pragma once
#include "DrawComponent.h"
#include "Vector2D.h"
#include <array>
#include <vector>

namespace planeta_engine {
	namespace components {
		class DrawLineComponent : public DrawComponent {
		public:
			DrawLineComponent();
			void Draw()override;
			/*アクセサ*/
			int width()const { return _width; }
			void width(int w) { _width = w; }
			double length()const { return _length; }
			void length(double l) { _length = l; }
		private:
			std::vector<Vector2D<double>> _wire_positions;
			int _width;
			double _length;
		};
	}
}