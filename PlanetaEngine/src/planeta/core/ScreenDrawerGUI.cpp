﻿#include <cassert>

#include "DxLib.h"

#include "planeta/buildin/RGraph.hpp"
#include "planeta/buildin/RFont.hpp"
#include "planeta/dxlib/DXUtility.hpp"

#include "Screen.hpp"
#include "ScreenDrawerGUI.hpp"

namespace plnt {
	void ScreenDrawerGUI::DrawGraph(const Vector2Di& position, const Vector2Di& size, const Vector2Dd& center_pivot, double rotation_rad, const RectAnglei& draw_area_on_graph, bool reverse, const std::shared_ptr<RGraph>& graph_resource) {
		screen_.ReserveDraw([position,size,draw_area_on_graph,reverse,graph_resource,rotation_rad,center_pivot]() {
			if (graph_resource == nullptr) { return; }
			double scale_x = (double)size.x / draw_area_on_graph.width();
			double scale_y = (double)size.y / draw_area_on_graph.height();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawRectRotaGraph3(
				position.x, position.y,
				draw_area_on_graph.x_min(), draw_area_on_graph.y_min(),
				size.x, size.y,
				static_cast<int>(draw_area_on_graph.width() * center_pivot.x), 
				static_cast<int>(draw_area_on_graph.height()  * center_pivot.y),
				scale_x, scale_y,
				rotation_rad, graph_resource->GetHandle(), true, reverse);
		});
	}

	void ScreenDrawerGUI::DrawWire(const std::vector<Vector2Di>& positions, int width, const plnt::Color& color) {
		screen_.ReserveDraw([positions,width,color]() {
			int dxc = dxlib::PEColorToDXColorHandle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			for (int i = 0; i < (int)positions.size() - 1; ++i) {
				DrawLine(positions[i].x, positions[i].y, positions[i + 1].x, positions[i + 1].y, dxc, width);
			}
		});
	}

	void ScreenDrawerGUI::DrawPolygon(const std::vector<Vector2Di>& positions, const std::vector<std::array<int, 3>>& indexes, const plnt::Color& color) {
		screen_.ReserveDraw([positions,indexes,color]() {
			int dxc = dxlib::PEColorToDXColorHandle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			try {
				for (size_t i = 0; i < indexes.size(); ++i) {
					const auto& v0 = positions.at(indexes[i][0]);
					const auto& v1 = positions.at(indexes[i][1]);
					const auto& v2 = positions.at(indexes[i][2]);
					DrawTriangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, dxc, true);
				}
			} catch (std::out_of_range&) {
				//インデックス番号が範囲外を指していた
				assert(false);
			}
		});
	}

	void ScreenDrawerGUI::DrawString(const Vector2Di& position, const Vector2Dd scale, const std::string& str, const plnt::Color& color, const plnt::Color& outline_color, const std::shared_ptr<RFont>& font_resource) {
		screen_.ReserveDraw([position,scale,str,color,outline_color,font_resource]() {
			if (font_resource == nullptr) { return; }
			DrawExtendStringToHandle(position.x, position.y, scale.x, scale.y, str.c_str(), dxlib::PEColorToDXColorHandle(color), font_resource->GetDXHandle(), dxlib::PEColorToDXColorHandle(outline_color));
		});
	}

	void ScreenDrawerGUI::DrawStringByDefaultFont(const Vector2Di& position, const Vector2Dd scale, const std::string& str, const plnt::Color& color, const plnt::Color& outline_color) {
		screen_.ReserveDraw([position, scale, str, color, outline_color]() {
			DrawExtendString(position.x, position.y, scale.x, scale.y, str.c_str(), dxlib::PEColorToDXColorHandle(color), dxlib::PEColorToDXColorHandle(outline_color));
		});
	}

}