#pragma once

#include <vector>
#include <memory>
#include <array>
#include "RectAngle.h"
#include "Color.h"

namespace planeta {
	class RGraph;
	class RFont;
	namespace private_ {
		class Screen;
	}
	/*! 画面上にGUIを描画するクラス*/
	class ScreenDrawerGUI {
	public:
		explicit ScreenDrawerGUI(private_::Screen& scr) :screen_(scr) {}
		/*! 画像を描画(描画位置、描画サイズ、中心位置、回転度、画像上の描画範囲、反転フラグ、画像リソース)*/
		void DrawGraph(const Vector2Di& position, const Vector2Di& size, const Vector2Dd& center_pivot, double rotation_rad, const RectAnglei& draw_area_on_graph, bool reverse, const std::shared_ptr<RGraph>& graph_resource);
		/*! ワイヤーを描画(点座標、幅(現状では無効)、色)*/
		void DrawWire(const std::vector<Vector2Di>& positions, int width, const planeta::Color& color);
		/*! 多角形を描画(点座標、三角形ポリゴンインデックス、色)*/
		void DrawPolygon(const std::vector<Vector2Di>& positions, const std::vector<std::array<int, 3>>& indexes, const planeta::Color& color);
		/*! 文字列を描画(描画位置、拡大度、描画文字列、色、縁色、フォント定義リソース)*/
		void DrawString(const Vector2Di& position, const Vector2Dd scale, const std::string& str, const planeta::Color& color, const planeta::Color& outline_color, const std::shared_ptr<RFont>& font_resource);
		/*! 文字列を描画(描画位置、拡大度、描画文字列、色、縁色)*/
		void DrawStringByDefaultFont(const Vector2Di& position, const Vector2Dd scale, const std::string& str, const planeta::Color& color, const planeta::Color& outline_color);
	private:
		private_::Screen& screen_;
	};
}
