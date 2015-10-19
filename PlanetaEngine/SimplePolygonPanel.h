#pragma once

#pragma once

#include <memory>
#include <vector>
#include <array>
#include "UIComponent.h"
#include "Color.h"

namespace planeta_engine {
	namespace ui_components {
		class SimplePolygonPanel : public game::UIComponent {
		public:
			void SetNumberOfVertexes(size_t n);
			/*指定インデックスの座標を設定。座標はコンポーネントの左上を(0.0,0.0)、右下を(1.0,1.0)とした座標で示す。*/
			bool SetVertexPosition(size_t idx, const Vector2D<double>& pos);
			/*頂点を設定。座標はコンポーネントの左上を(0.0,0.0)、右下を(1.0,1.0)とした座標で示す。*/
			void SetVertexes(const std::vector<Vector2D<double>>& vs);
			/*アクセサ*/
			/*塗りつぶし色を取得*/
			const core::Color& fill_color()const { return fill_color_; }
			/*塗りつぶし色を設定*/
			void fill_color(const core::Color& c) { fill_color_ = c; }
			/*縁を描画するかのフラグを取得*/
			bool is_draw_edge()const { return is_draw_outline_; }
			/*縁を描画するかのフラグを設定*/
			void is_draw_edge(bool f) { is_draw_outline_ = f; }
			/*枠線の色を取得*/
			const core::Color& outline_color()const { return outline_color_; }
			/*縁線の色を設定*/
			void outline_color(const core::Color& c) { outline_color_ = c; }
			/*縁線の太さを取得*/
			const int outline_width()const { return outline_width_; }
			/*枠線の太さを設定*/
			void outline_width(int w) { outline_width_ = w; }
		private:
			virtual void UpdateProc() override;
			virtual void DrawProc(const utility::RectAngle<int>& draw_area) override;
			bool points_update_flag_ = true;
			/*Polygon情報*/
			core::Color fill_color_;
			std::vector<Vector2D<double>> relative_points_;
			std::vector<Vector2D<int>> absolute_points_;
			std::vector<std::array<int, 3>> indexes_;
			void UpdateAbsolutePoints_(const utility::RectAngle<int>& draw_area);
			void UpdatePorygonIndex_();
			/*縁情報*/
			bool is_draw_outline_ = false;
			core::Color outline_color_;
			int outline_width_;

			void OnRectAngleUpdated()override;
		};
	}
}
