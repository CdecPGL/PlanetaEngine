#pragma once

#include <vector>
#include <memory>
#include <array>
#include "RectAngle.h"
#include "Color.h"

namespace planeta {
	class RGraph;
	class RFontDefinition;
	namespace private_ {
		class Screen;
	}
	class ScreenDrawerGUI {
	public:
		explicit ScreenDrawerGUI(private_::Screen& scr) :screen_(scr) {}
		/*画像を描画(描画範囲、画像上の描画範囲、反転フラグ、画像リソース)*/
		void DrawGraph(const RectAngle<int>& draw_area, const RectAngle<int>& draw_area_on_graph, bool reverse, const std::shared_ptr<RGraph>& graph_resource);
		/*ワイヤーを描画(点座標、幅(現状では無効)、色)*/
		void DrawWire(const std::vector<Vector2Di>& positions, int width, const planeta::Color& color);
		/*多角形を描画(点座標、三角形ポリゴンインデックス、色)*/
		void DrawPolygon(const std::vector<Vector2Di>& positions, const std::vector<std::array<int, 3>>& indexes, const planeta::Color& color);
		/*文字列を描画(描画位置、拡大度、描画文字列、色、縁色、フォント定義リソース)*/
		void DrawString(const Vector2Di& position, const Vector2Dd scale, const std::string& str, const planeta::Color& color, const planeta::Color& outline_color, const std::shared_ptr<RFontDefinition>& font_definition_resource);
	private:
		private_::Screen& screen_;
	};
}
