#pragma once

#include <memory>
#include <vector>
#include <array>
#include "CDraw2D.h"
#include "RectAngle.h"
#include "Vertex2D.h"

namespace planeta {
	namespace core {
		class GraphDrawData2D;
	}
	namespace resources {
		class GraphResource;
	}
	class CDrawGraph2D final : public CDraw2D {
	public:
		CDrawGraph2D();
		~CDrawGraph2D();
		bool SetGraphResource(const std::string& resource_id);
		/*アクセサ*/
		const util::RectAngle<int>& draw_area()const { return _draw_area; }
		CDrawGraph2D& draw_area(const util::RectAngle<int>& rect) { _draw_area = rect; _UpdateUVPosition(); return *this; }
		const bool reverse()const { return reverse_; }
		CDrawGraph2D& reverse(bool rev) { reverse_ = rev; _UpdateUVPosition(); return *this; }
		const Vector2Dd& graph_center()const { return graph_center_; }
		void graph_center(const Vector2Dd& pos) { graph_center_ = pos; }
	private:
		/*反転描画フラグ*/
		bool reverse_ = false;
		/*画像上で表示する範囲*/
		util::RectAngle<int> _draw_area;
		/*画像の中心位置*/
		Vector2Dd graph_center_ = Vector2Dd(0.5, 0.5);
		/*画像描画データ*/
		std::shared_ptr<core::GraphDrawData2D> graph_draw_data_;
		/*ポリゴン情報更新*/
		void _UpdatePolygon();
		/*UV座標更新*/
		void _UpdateUVPosition();
		/*描画処理*/
		void DrawProc(ScreenDrawer2D& drawer)override;
	};
	PE_REGISTER_GAMEOBJECTCOMPONENT(CDrawGraph2D);
}