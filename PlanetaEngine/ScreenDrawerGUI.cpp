#include <cassert>

#include "DxLib.h"

#include "ScreenDrawerGUI.h"
#include "GraphResource.h"
#include "DXUtility.h"
#include "FontDefinitionResource.h"

namespace planeta_engine {
	void ScreenDrawerGUI::DrawGraph(const utility::RectAngle<int>& draw_area, const utility::RectAngle<int>& draw_area_on_graph, bool reverse, const std::shared_ptr<resources::GraphResource>& graph_resource) {
		if (graph_resource == nullptr) { return; }
		double scale_x = (double)draw_area.size.x / draw_area_on_graph.size.x;
		double scale_y = (double)draw_area.size.y / draw_area_on_graph.size.y;
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DrawRectRotaGraph3(draw_area.position.x, draw_area.position.y, draw_area_on_graph.position.x, draw_area_on_graph.position.y, draw_area.size.x, draw_area.size.y, draw_area_on_graph.size.x / 2, draw_area_on_graph.size.y / 2, scale_x, scale_y, 0.0, graph_resource->GetHandle(), true, reverse);
	}

	void ScreenDrawerGUI::DrawWire(const std::vector<Vector2D<int>>& positions, int width, const planeta_engine::Color& color) {
		using namespace utility::dx;
		int dxc = PEColorToDXColorHandle(color);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
		for (int i = 0; i < (int)positions.size() - 1; ++i) {
			DrawLine(positions[i].x, positions[i].y, positions[i + 1].x, positions[i + 1].y, dxc, width);
		}
	}

	void ScreenDrawerGUI::DrawPolygon(const std::vector<Vector2D<int>>& positions, const std::vector<std::array<int, 3>>& indexes, const planeta_engine::Color& color) {
		using namespace utility::dx;
		int dxc = PEColorToDXColorHandle(color);
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
	}

	void ScreenDrawerGUI::DrawString(const Vector2D<int>& position, const Vector2D<double> scale, const std::string& str, const planeta_engine::Color& color, const planeta_engine::Color& outline_color, const std::shared_ptr<resources::FontDefinitionResource>& font_definition_resource) {
		if (font_definition_resource == nullptr) { return; }
		using namespace utility::dx;
		DrawExtendStringToHandle(position.x, position.y, scale.x, scale.y, str.c_str(), PEColorToDXColorHandle(color), font_definition_resource->GetHandle(), PEColorToDXColorHandle(outline_color));
	}
}