﻿#include "../core/screen_drawer_2d.hpp"
#include "../core/i_game_object.hpp"
#include "../core/matrix_22.hpp"
#include "c_transform_2d.hpp"
#include "c_draw_polygon_2d.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_draw_polygon_2d) {
		registerer
			.write_only_property("vertexes", &c_draw_polygon_2d::vertexes)
			.shallow_copy_target(&c_draw_polygon_2d::points_)
			.shallow_copy_target(&c_draw_polygon_2d::vertexes_)
			.shallow_copy_target(&c_draw_polygon_2d::indexes_);
	}

	void c_draw_polygon_2d::draw_proc(screen_drawer_2d &drawer) {
		update_vertex();
		drawer.draw_polygon(vertexes_, indexes_, color());
	}

	void c_draw_polygon_2d::set_number_of_vertexes(const size_t n) {
		points_.resize(n);
		vertexes_.resize(n);
		update_polygon_index();
	}

	bool c_draw_polygon_2d::set_vertex_position(const size_t idx, const vector_2dd &pos) {
		if (idx >= points_.size()) { return false; } else {
			points_[idx] = static_cast<vector_2df>(pos);
			return true;
		}
	}

	void c_draw_polygon_2d::vertexes(const std::vector<vector_2dd> &vs) {
		points_.resize(vs.size());
		for (size_t i = 0; i < points_.size(); ++i) { points_[i] = static_cast<vector_2df>(vs[i]); }
		vertexes_.resize(points_.size());
		update_polygon_index();
	}

	void c_draw_polygon_2d::update_polygon_index() {
		indexes_.clear();
		if (points_.size() < 3) { return; }
		for (size_t i = 0; i < points_.size() - 2; ++i) {
			indexes_.push_back({0, static_cast<int>(i) + 1, static_cast<int>(i) + 2});
		}
	}

	void c_draw_polygon_2d::update_vertex() {
		for (size_t i = 0; i < points_.size(); ++i) {
			vector_2dd relative_pos0(points_[i].x * relative_scale().x, points_[i].y * relative_scale().y);
			//描画コンポーネントの拡大度を適用
			relative_pos0 = math::rotation_transform(relative_rotation_rad(), relative_pos0); //描画コンポーネントの回転度を適用
			auto relative_pos1 = vector_2dd((relative_position() + relative_pos0).x * transform2d_->scale().x,
			                                (relative_position() + relative_pos0).y * transform2d_->scale().y);
			//トランスフォームの拡大度を適用
			auto pos = transform2d_->position() + math::rotation_transform(
				transform2d_->rotation_rad(), relative_pos1); //トランスフォームの回転度を適用
			vertexes_[i] = static_cast<vector_2df>(pos);
		}
	}
}
