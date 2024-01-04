#include <cassert>

#include "DxLib.h"

#include "..\buildin\r_graph.hpp"
#include "..\buildin\r_font.hpp"
#include "../dxlib/dx_utility.hpp"

#include "screen.hpp"
#include "screen_drawer_gui.hpp"

namespace plnt {
	void screen_drawer_gui::draw_graph(const vector_2di &position, const vector_2di &size, const vector_2dd &center_pivot,
	                                double rotation_rad, const rectangle_i &draw_area_on_graph, bool reverse,
	                                const std::shared_ptr<r_graph> &graph_resource) const {
		screen_.reserve_draw([position,size,draw_area_on_graph,reverse,graph_resource,rotation_rad,center_pivot] {
			if (graph_resource == nullptr) { return; }
			const double scale_x = static_cast<double>(size.x) / draw_area_on_graph.width();
			const double scale_y = static_cast<double>(size.y) / draw_area_on_graph.height();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawRectRotaGraph3(
				position.x, position.y,
				draw_area_on_graph.x_min(), draw_area_on_graph.y_min(),
				size.x, size.y,
				static_cast<int>(draw_area_on_graph.width() * center_pivot.x),
				static_cast<int>(draw_area_on_graph.height() * center_pivot.y),
				scale_x, scale_y,
				rotation_rad, graph_resource->get_handle(), true, reverse);
		});
	}

	void screen_drawer_gui::draw_wire(const std::vector<vector_2di> &positions, int width, const color &color) const {
		screen_.reserve_draw([positions,width,color] {
			const auto dxc = dxlib::pe_color_to_dx_color_handle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			for (int i = 0; i < static_cast<int>(positions.size()) - 1; ++i) {
				DrawLine(positions[i].x, positions[i].y, positions[i + 1].x, positions[i + 1].y, dxc, width);
			}
		});
	}

	void screen_drawer_gui::draw_polygon(const std::vector<vector_2di> &positions,
	                                  const std::vector<std::array<int, 3>> &indexes, const color &color) const {
		screen_.reserve_draw([positions,indexes,color] {
			const auto dxc = dxlib::pe_color_to_dx_color_handle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			try {
				for (auto index : indexes) {
					const auto &v0 = positions.at(index[0]);
					const auto &v1 = positions.at(index[1]);
					const auto &v2 = positions.at(index[2]);
					DrawTriangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, dxc, true);
				}
			} catch (std::out_of_range &) {
				//インデックス番号が範囲外を指していた
				assert(false);
			}
		});
	}

	void screen_drawer_gui::draw_string(const vector_2di &position, const vector_2dd &scale, const std::string &str,
	                                 const color &color, const plnt::color &outline_color,
	                                 const std::shared_ptr<r_font> &font_resource) const {
		screen_.reserve_draw([position,scale,str,color,outline_color,font_resource] {
			if (font_resource == nullptr) { return; }
			DrawExtendStringToHandle(position.x, position.y, scale.x, scale.y, str.c_str(),
			                         dxlib::pe_color_to_dx_color_handle(color), font_resource->get_dx_handle(),
			                         dxlib::pe_color_to_dx_color_handle(outline_color));
		});
	}

	void screen_drawer_gui::draw_string_by_default_font(const vector_2di &position, const vector_2dd &scale,
	                                              const std::string &str, const color &color,
	                                              const plnt::color &outline_color) const {
		screen_.reserve_draw([position, scale, str, color, outline_color] {
			DrawExtendString(position.x, position.y, scale.x, scale.y, str.c_str(),
			                 dxlib::pe_color_to_dx_color_handle(color), dxlib::pe_color_to_dx_color_handle(outline_color));
		});
	}
}
