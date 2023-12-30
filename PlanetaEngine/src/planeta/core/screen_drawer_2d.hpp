#pragma once

#include <memory>
#include <vector>
#include <array>

#include "Vector2D.hpp"
// ReSharper disable once CppUnusedIncludeDirective
#include "color.hpp"

namespace plnt {
	namespace private_ {
		class screen;
		class graph_draw_data_2d;
	}

	class screen_drawer_2d {
	public:
		explicit screen_drawer_2d(private_::screen &scr) : screen_(scr) { }

		/*画像の描画(画像描画データ)*/
		void draw_graph(const std::shared_ptr<private_::graph_draw_data_2d> &graph_draw_data) const;
		/*ワイヤーの描画(点座標、幅(現状では無効)、色)*/
		void draw_wire(const std::vector<vector_2df> &positions, double width, const color &color) const;
		/*多角形の描画(点座標、三角形ポリゴンインデックス、色)*/
		void draw_polygon(const std::vector<vector_2df> &positions, const std::vector<std::array<int, 3>> &indexes,
		                 const color &color) const;
		/*円の描画*/
		void draw_circle(const vector_2df &position, float radius, const color &color) const;

	private:
		private_::screen &screen_;
	};
}
