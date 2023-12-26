﻿#include "planeta/core/SimplePolygonPanel.hpp"

#include "planeta/core/LogUtility.hpp"
#include "planeta/core/ScreenDrawerGUI.hpp"

namespace plnt {
	//namespace gui_components {

	//	void SimplePolygonPanel::UpdateProc()
	//	{

	//	}

	//	void SimplePolygonPanel::DrawProc(const utility::RectAngle<int>& draw_area, ScreenDrawerGUI& drawer)
	//	{
	//		if (points_update_flag_) { UpdateAbsolutePoints_(draw_area); points_update_flag_ = false; }
	//		drawer.DrawPolygon(absolute_points_, indexes_, fill_color());
	//		if (is_draw_outline_) {
	//			drawer.DrawWire(absolute_points_, outline_width_, outline_color());
	//		}
	//	}

	//	void SimplePolygonPanel::SetNumberOfVertexes(size_t n)
	//	{
	//		relative_points_.resize(n);
	//		absolute_points_.resize(n + 1);
	//		UpdatePorygonIndex_();
	//		points_update_flag_ = true;
	//	}

	//	bool SimplePolygonPanel::SetVertexPosition(size_t idx, const Vector2Dd& pos)
	//	{
	//		if (idx >= relative_points_.size()) { return false; }
	//		else {
	//			relative_points_[idx] = pos;
	//			points_update_flag_ = true;
	//			return true;
	//		}
	//	}

	//	void SimplePolygonPanel::SetVertexes(const std::vector<Vector2Dd>& vs)
	//	{
	//		relative_points_ = vs;
	//		absolute_points_.resize(vs.size() + 1);
	//		UpdatePorygonIndex_();
	//		points_update_flag_ = true;
	//	}

	//	void SimplePolygonPanel::UpdatePorygonIndex_()
	//	{
	//		indexes_.clear();
	//		if (relative_points_.size() < 3) { return; }
	//		for (size_t i = 0; i < relative_points_.size() - 2; ++i) {
	//			indexes_.push_back({ 0,(int)i + 1,(int)i + 2 });
	//		}
	//	}

	//	void SimplePolygonPanel::UpdateAbsolutePoints_(const utility::RectAngle<int>& draw_area)
	//	{
	//		for (size_t i = 0; i < relative_points_.size(); ++i) {
	//			//コンポーネント左上からの相対位置とコンポーネントの描画エリアから画面上の絶対位置を求める
	//			absolute_points_[i] = draw_area.position + Vector2Di((int)(draw_area.size.x*relative_points_[i].x), (int)(draw_area.size.y*relative_points_[i].y));
	//		}
	//		absolute_points_[absolute_points_.size() - 1] = absolute_points_[0];
	//	}
	//}
}
