#include "planeta/core/game.hpp"
#include "planeta/core/i_resource_manager.hpp"
#include "../core/screen_drawer_2d.hpp"
#include "../core/log_utility.hpp"
#include "../core/matrix_22.hpp"
#include "../core/graph_draw_data_2d.hpp"
#include "c_draw_graph_2d.hpp"
#include "r_graph.hpp"
#include "c_transform_2d.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_draw_graph_2d) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_graph_2d, draw_area)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_graph_2d, reverse)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_graph_2d, graph_center)
			.write_only_property("graph_resource_id", &c_draw_graph_2d::graph_resource_id)
			.shallow_copy_target(&c_draw_graph_2d::draw_area_)
			.shallow_copy_target(&c_draw_graph_2d::reverse_)
			.shallow_copy_target(&c_draw_graph_2d::graph_center_)
			.deep_copy_target(&c_draw_graph_2d::graph_draw_data_);
	}

	/*頂点は[0]左下,[1]右下,[2]右上,[3]左上とする*/
	c_draw_graph_2d::c_draw_graph_2d() : graph_draw_data_(std::make_shared<private_::graph_draw_data_2d>()) {
		graph_draw_data_->set_vertex_count(4);
		graph_draw_data_->set_polygon_indexes({{0, 1, 3}, {1, 3, 2}});
	}

	c_draw_graph_2d::~c_draw_graph_2d() = default;

	bool c_draw_graph_2d::set_graph_resource(const std::string &resource_id) {
		if (const auto res = game::instance().resource_manager()->get_resource_by_id<r_graph>(resource_id)) {
			graph_draw_data_->set_graph_resource(res);
			draw_area_.set(vector_2di(0, 0), vector_2di(res->size().x, res->size().y));
			update_uv_position();
			return true;
		}
		PE_LOG_ERROR("リソースの取得に失敗しました。(ResourceID: ", resource_id, ")");
		return false;
	}

	void c_draw_graph_2d::draw_proc(screen_drawer_2d &drawer) {
		if (graph_draw_data_->graph_resource()) {
			update_polygon();
			drawer.draw_graph(graph_draw_data_);
		}
	}

	void c_draw_graph_2d::update_polygon() const {
		//頂点位置の更新
		//描画拡大度を考慮した縦横の長さ
		const double d_width(static_cast<double>(draw_area_.width()) * get_draw_scale().x), d_height(
			       static_cast<double>(draw_area_.height()) * get_draw_scale().y);
		//ゲームオブジェクトの形状情報と画像の表示位置から画像の表示中心位置を求める
		const vector_2dd center_position = get_draw_center_position();
		//左上の頂点ベクトル
		const vector_2dd left_up_vertex_vec(d_width * -graph_center_.x, d_height * graph_center_.y);
		//右上の頂点ベクトル
		const vector_2dd right_up_vertex_vec(d_width * (1.0 - graph_center_.x), d_height * graph_center_.y);
		//左下の頂点ベクトル
		const vector_2dd left_down_vertex_vec(d_width * -graph_center_.x, d_height * (graph_center_.y - 1.0));
		//右下の頂点ベクトル
		const vector_2dd right_down_vertex_vec(d_width * (1.0 - graph_center_.x), d_height * (graph_center_.y - 1.0));
		//画像の回転度とゲームオブジェクトの回転度、表示中心位置から各頂点を求める
		graph_draw_data_->set_vertex_position(
			0, static_cast<vector_2df>(center_position + math::rotation_transform(
				get_draw_rotation_red(), left_down_vertex_vec)));
		graph_draw_data_->set_vertex_position(
			1, static_cast<vector_2df>(center_position + math::rotation_transform(
				get_draw_rotation_red(), right_down_vertex_vec)));
		graph_draw_data_->set_vertex_position(
			2, static_cast<vector_2df>(center_position + math::rotation_transform(
				get_draw_rotation_red(), right_up_vertex_vec)));
		graph_draw_data_->set_vertex_position(
			3, static_cast<vector_2df>(center_position + math::rotation_transform(
				get_draw_rotation_red(), left_up_vertex_vec)));
		//色
		for (int i = 0; i < 4; ++i) { graph_draw_data_->set_vertex_color(i, color()); }
	}

	void c_draw_graph_2d::update_uv_position() const {
		//UV座標の更新
		vector_2df uvs[4];
		const auto g_size = graph_draw_data_->graph_resource()->size();
		//左下
		uvs[0].x = static_cast<float>(reverse_ ? draw_area_.x_max() + 1 : draw_area_.x_min()) / g_size.x;
		uvs[0].y = static_cast<float>(draw_area_.y_max() + 1) / g_size.y;
		//右下
		uvs[1].x = static_cast<float>(reverse_ ? draw_area_.x_min() : draw_area_.x_max() + 1) / g_size.x;
		uvs[1].y = static_cast<float>(draw_area_.y_max() + 1) / g_size.y;
		//右上
		uvs[2].x = static_cast<float>(reverse_ ? draw_area_.x_min() : draw_area_.x_max() + 1) / g_size.x;
		uvs[2].y = static_cast<float>(draw_area_.y_min()) / g_size.y;
		//左上
		uvs[3].x = static_cast<float>(reverse_ ? draw_area_.x_max() + 1 : draw_area_.x_min()) / g_size.x;
		uvs[3].y = static_cast<float>(draw_area_.y_min()) / g_size.y;

		for (int i = 0; i < 4; ++i) { graph_draw_data_->set_vertex_uv(i, uvs[i]); }
	}

	c_draw_graph_2d &c_draw_graph_2d::graph_resource_id(const std::string &res_id) {
		set_graph_resource(res_id);
		return *this;
	}
}
