﻿#pragma once

#include <memory>
#include <vector>
#include <array>
#include "Vector2D.hpp"
#include "Color.hpp"

namespace plnt {
	namespace private_ {
		class Screen;
		class GraphDrawData2D;
	}
	class ScreenDrawer2D {
	public:
		explicit ScreenDrawer2D(private_::Screen& scr) :screen_(scr) {}
		/*画像の描画(画像描画データ)*/
		void DrawGraph(const std::shared_ptr<private_::GraphDrawData2D>& graph_draw_data);
		/*ワイヤーの描画(点座標、幅(現状では無効)、色)*/
		void DrawWire(const std::vector<Vector2Df>& positions, double width, const plnt::Color& color);
		/*多角形の描画(点座標、三角形ポリゴンインデックス、色)*/
		void DrawPolygon(const std::vector<Vector2Df>& positions, const std::vector<std::array<int, 3>>& indexes, const plnt::Color& color);
		/*円の描画*/
		void DrawCircle(const Vector2Df position, float radius, const Color& color);
	private:
		private_::Screen& screen_;
	};
}
