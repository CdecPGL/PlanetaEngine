#include "planeta/core/game.hpp"
#include "planeta/core/i_resource_manager.hpp"
#include "planeta/core/ScreenDrawer2D.hpp"
#include "..\core\log_utility.hpp"
#include "..\core\i_game_object.hpp"
#include "..\core\matrix_22.hpp"
#include "..\math\math_constant.hpp"
#include "..\core\graph_draw_data_2d.hpp"

#include "CDrawGraph2D.hpp"
#include "RGraph.hpp"
#include "CTransform2D.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CDrawGraph2D) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CDrawGraph2D, draw_area)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDrawGraph2D, reverse)
			.PE_REFLECTABLE_CLASS_PROPERTY(CDrawGraph2D, graph_center)
			.write_only_property("graph_resource_id", &CDrawGraph2D::graph_resource_id)
			.shallow_copy_target(&CDrawGraph2D::_draw_area)
			.shallow_copy_target(&CDrawGraph2D::reverse_)
			.shallow_copy_target(&CDrawGraph2D::graph_center_)
			.deep_copy_target(&CDrawGraph2D::graph_draw_data_);
	}

	/*頂点は[0]左下,[1]右下,[2]右上,[3]左上とする*/
	CDrawGraph2D::CDrawGraph2D() : graph_draw_data_(std::make_shared<private_::graph_draw_data_2d>()) {
		graph_draw_data_->set_vertex_count(4);
		graph_draw_data_->set_polygon_indexes({{0, 1, 3}, {1, 3, 2}});
	}

	CDrawGraph2D::~CDrawGraph2D() = default;

	bool CDrawGraph2D::SetGraphResource(const std::string &resource_id) {
		auto res = game::instance().resource_manager()->get_resource_by_id<RGraph>(resource_id);
		if (res) {
			graph_draw_data_->set_graph_resource(res);
			_draw_area.set(Vector2Di(0, 0), Vector2Di(res->size().x, res->size().y));
			_UpdateUVPosition();
			return true;
		} else {
			PE_LOG_ERROR("リソースの取得に失敗しました。(ResourceID: ", resource_id, ")");
			return false;
		}
	}

	void CDrawGraph2D::DrawProc(ScreenDrawer2D &drawer) {
		if (graph_draw_data_->graph_resource()) {
			_UpdatePolygon();
			drawer.DrawGraph(graph_draw_data_);
		}
	}

	void CDrawGraph2D::_UpdatePolygon() {
		//頂点位置の更新
		//描画拡大度を考慮した縦横の長さ
		double dwidth((double)_draw_area.width() * GetDrawScale().x), dheight(
			       (double)_draw_area.height() * GetDrawScale().y);
		//ゲームオブジェクトの形状情報と画像の表示位置から画像の表示中心位置を求める
		Vector2Dd center_position = GetDrawCenterPosition();
		//左上の頂点ベクトル
		Vector2Dd left_up_vertex_vec(dwidth * (-graph_center_.x), dheight * graph_center_.y);
		//右上の頂点ベクトル
		Vector2Dd right_up_vertex_vec(dwidth * (1.0 - graph_center_.x), dheight * graph_center_.y);
		//左下の頂点ベクトル
		Vector2Dd left_down_vertex_vec(dwidth * (-graph_center_.x), dheight * (graph_center_.y - 1.0));
		//右下の頂点ベクトル
		Vector2Dd right_down_vertex_vec(dwidth * (1.0 - graph_center_.x), dheight * (graph_center_.y - 1.0));
		//画像の回転度とゲームオブジェクトの回転度、表示中心位置から各頂点を求める
		graph_draw_data_->set_vertex_position(
			0, static_cast<Vector2Df>(center_position + math::rotation_transform(
				GetDrawRotationRed(), left_down_vertex_vec)));
		graph_draw_data_->set_vertex_position(
			1, static_cast<Vector2Df>(center_position + math::rotation_transform(
				GetDrawRotationRed(), right_down_vertex_vec)));
		graph_draw_data_->set_vertex_position(
			2, static_cast<Vector2Df>(center_position + math::rotation_transform(
				GetDrawRotationRed(), right_up_vertex_vec)));
		graph_draw_data_->set_vertex_position(
			3, static_cast<Vector2Df>(center_position + math::rotation_transform(
				GetDrawRotationRed(), left_up_vertex_vec)));
		//色
		for (int i = 0; i < 4; ++i) { graph_draw_data_->set_vertex_color(i, color()); }
	}

	void CDrawGraph2D::_UpdateUVPosition() {
		//UV座標の更新
		Vector2Df uvs[4];
		auto g_size = graph_draw_data_->graph_resource()->size();
		//左下
		uvs[0].x = (float)(reverse_ ? (_draw_area.x_max() + 1) : _draw_area.x_min()) / g_size.x;
		uvs[0].y = (float)(_draw_area.y_max() + 1) / g_size.y;
		//右下
		uvs[1].x = (float)(reverse_ ? _draw_area.x_min() : (_draw_area.x_max() + 1)) / g_size.x;
		uvs[1].y = (float)(_draw_area.y_max() + 1) / g_size.y;
		//右上
		uvs[2].x = (float)(reverse_ ? _draw_area.x_min() : (_draw_area.x_max() + 1)) / g_size.x;
		uvs[2].y = (float)_draw_area.y_min() / g_size.y;
		//左上
		uvs[3].x = (float)(reverse_ ? (_draw_area.x_max() + 1) : _draw_area.x_min()) / g_size.x;
		uvs[3].y = (float)_draw_area.y_min() / g_size.y;

		for (int i = 0; i < 4; ++i) { graph_draw_data_->set_vertex_uv(i, uvs[i]); }
	}

	CDrawGraph2D &CDrawGraph2D::graph_resource_id(const std::string &res_id) {
		SetGraphResource(res_id);
		return *this;
	}
}
