﻿#pragma once

#include <vector>
#include <memory>
#include <array>
#include "rectangle.hpp"
// ReSharper disable once CppUnusedIncludeDirective
#include "color.hpp"

namespace plnt {
	class r_graph;
	class r_font;

	namespace private_ {
		class screen;
	}

	/*! 画面上にGUIを描画するクラス*/
	class screen_drawer_gui {
	public:
		explicit screen_drawer_gui(private_::screen &scr) : screen_(scr) { }

		/*! 画像を描画(描画位置、描画サイズ、中心位置、回転度、画像上の描画範囲、反転フラグ、画像リソース)*/
		void draw_graph(const vector_2di &position, const vector_2di &size, const vector_2dd &center_pivot,
		               double rotation_rad, const rectangle_i &draw_area_on_graph, bool reverse,
		               const std::shared_ptr<r_graph> &graph_resource) const;
		/*! ワイヤーを描画(点座標、幅(現状では無効)、色)*/
		void draw_wire(const std::vector<vector_2di> &positions, int width, const color &color) const;
		/*! 多角形を描画(点座標、三角形ポリゴンインデックス、色)*/
		void draw_polygon(const std::vector<vector_2di> &positions, const std::vector<std::array<int, 3>> &indexes,
		                 const color &color) const;
		/*! 文字列を描画(描画位置、拡大度、描画文字列、色、縁色、フォント定義リソース)*/
		void draw_string(const vector_2di &position, const vector_2dd &scale, const std::string &str,
		                const color &color, const plnt::color &outline_color,
		                const std::shared_ptr<r_font> &font_resource) const;
		/*! 文字列を描画(描画位置、拡大度、描画文字列、色、縁色)*/
		void draw_string_by_default_font(const vector_2di &position, const vector_2dd &scale, const std::string &str,
		                             const color &color, const plnt::color &outline_color) const;

	private:
		private_::screen &screen_;
	};
}
