#pragma once

#include <memory>
#include <vector>
#include <array>
#include "Vector2D.h"
#include "Color.h"

namespace planeta_engine {
	namespace core {
		class Screen;
		class GraphDrawData;
		class ScreenDrawer2D {
		public:
			explicit ScreenDrawer2D(Screen& scr) :screen_(scr) {}
			/*画像の描画(画像描画データ)*/
			void DrawGraph(const GraphDrawData& graph_draw_data);
			/*ワイヤーの描画(点座標、幅(現状では無効)、色)*/
			void DrawWire(const std::vector<Vector2D<double>>& positions, double width, const Color& color);
			/*多角形の描画(点座標、三角形ポリゴンインデックス、色)*/
			void DrawPolygon(const std::vector<Vector2D<double>>& positions, const std::vector<std::array<int, 3>>& indexes, const Color& color);
		private:
			Screen& screen_;
		};
	}
}
