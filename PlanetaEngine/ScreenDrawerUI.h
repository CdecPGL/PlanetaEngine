#pragma once

#include <vector>
#include <memory>
#include <array>
#include "RectAngle.h"
#include "Color.h"

namespace planeta_engine {
	namespace resources {
		class GraphResource;
		class FontDefinitionResource;
	}
	class ScrenDrawerUI {
	public:
		/*UI画像を描画(描画範囲、画像上の描画範囲、反転フラグ、画像リソース)*/
		void DrawUIGraph(const utility::RectAngle<int>& draw_area, const utility::RectAngle<int>& draw_area_on_graph, bool reverse, const std::shared_ptr<resources::GraphResource>& graph_resource);
		/*UIワイヤーを描画(点座標、幅(現状では無効)、色)*/
		void DrawUIWire(const std::vector<Vector2D<int>>& positions, int width, const planeta_engine::Color& color);
		/*UI多角形を描画(点座標、三角形ポリゴンインデックス、色)*/
		void DrawUIPolygon(const std::vector<Vector2D<int>>& positions, const std::vector<std::array<int, 3>>& indexes, const planeta_engine::Color& color);
		/*文字列を描画(描画位置、拡大度、描画文字列、色、縁色、フォント定義リソース)*/
		void DrawUIString(const Vector2D<int>& position, const Vector2D<double> scale, const std::string& str, const planeta_engine::Color& color, const planeta_engine::Color& outline_color, const std::shared_ptr<resources::FontDefinitionResource>& font_definition_resource);
	};
}
