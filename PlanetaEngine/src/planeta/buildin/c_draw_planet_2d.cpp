#include <cmath>

#include "planeta/core/game.hpp"
#include "../core/screen_drawer_2d.hpp"
#include "planeta/core/i_resource_manager.hpp"
#include "../core/i_game_object.hpp"
#include "../core/log_utility.hpp"
#include "../core/graph_draw_data_2d.hpp"
#include "../math/math_constant.hpp"
#include "c_draw_planet_2d.hpp"
#include "c_planet.hpp"
#include "r_graph.hpp"

namespace {
	constexpr unsigned int default_horizontal_separation(90);
	constexpr unsigned int default_vertical_separation(5);
}

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_draw_planet_2d) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_planet_2d, horizontal_separation)
			.PE_REFLECTABLE_CLASS_PROPERTY(c_draw_planet_2d, vertical_separation)
			.PE_REFLECTABLE_CLASS_PROPERTY_GS(c_draw_planet_2d, texture_mapping_mode)
			.write_only_property("graph_resource_id", &c_draw_planet_2d::graph_resource_id)
			.shallow_copy_target(&c_draw_planet_2d::horizontal_separation_)
			.shallow_copy_target(&c_draw_planet_2d::vertical_separation_)
			.deep_copy_target(&c_draw_planet_2d::graph_draw_data_)
			.copy_handler([](const c_draw_planet_2d &src, c_draw_planet_2d &dst) {
				dst.set_texture_mapping_mode(src.get_texture_mapping_mode());
			});
	}

	c_draw_planet_2d::c_draw_planet_2d() : horizontal_separation_(default_horizontal_separation),
	                                 vertical_separation_(default_vertical_separation),
	                                 graph_draw_data_(std::make_shared<private_::graph_draw_data_2d>()) {
		set_texture_mapping_mode(texture_mapping_mode::round);
	}

	c_draw_planet_2d::~c_draw_planet_2d() = default;

	void c_draw_planet_2d::draw_proc(screen_drawer_2d &drawer) {
		if (graph_draw_data_->graph_resource()) {
			if (set_polygon_flag_) {
				set_polygon();
				set_polygon_flag_ = false;
			}
			update_polygon();
			drawer.draw_graph(graph_draw_data_);
		}
	}

	bool c_draw_planet_2d::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		planet_component_.reset(com_getter.get_component<c_planet>());
		if (planet_component_ == nullptr) {
			PE_LOG_ERROR("初期化に失敗しました。PlanetComponentを取得できませんでした。");
			return false;
		}
		return true;
	}

	void c_draw_planet_2d::on_initialized() {
		super::on_initialized();
		set_polygon();
	}

	void c_draw_planet_2d::on_finalized() noexcept { super::on_finalized(); }

	void c_draw_planet_2d::set_polygon_roundly() const {
		//頂点とインデックスのサイズ調整
		graph_draw_data_->set_vertex_count((vertical_separation_ + 1) * (horizontal_separation_ + 1));
		graph_draw_data_->set_polygon_count(
			(vertical_separation_ - 1) * horizontal_separation_ * 2 + horizontal_separation_);
		//画像と頂点の設定
		for (unsigned int i = 0; i < horizontal_separation_ + 1; ++i) {
			//水平方向は座標系正回りにセットしていく
			for (unsigned int j = 0; j < vertical_separation_ + 1; ++j) {
				//垂直方向は上から順にセットしていく
				//テクスチャ座標のセット
				vector_2df uv;
				uv.x = static_cast<float>(i) / horizontal_separation_;
				uv.y = static_cast<float>(j) / vertical_separation_;
				graph_draw_data_->set_vertex_uv((vertical_separation_ + 1) * i + j, uv);
			}
		}
		//インデックスの設定
		for (unsigned int i = 0; i < horizontal_separation_; ++i) {
			//中心以外はポリゴンを2枚ずつ張る
			for (unsigned int j = 0; j < vertical_separation_ - 1; ++j) {
				private_::graph_draw_data_2d::polygon_index_type poly1, poly2;
				poly1[0] = i * (vertical_separation_ + 1) + j;
				poly1[1] = i * (vertical_separation_ + 1) + j + 1;
				poly1[2] = (i + 1) * (vertical_separation_ + 1) + j;
				poly2[0] = i * (vertical_separation_ + 1) + j + 1;
				poly2[1] = (i + 1) * (vertical_separation_ + 1) + j;
				poly2[2] = (i + 1) * (vertical_separation_ + 1) + j + 1;
				graph_draw_data_->set_polygon_index(i * (vertical_separation_ * 2 - 1) + j * 2, poly1);
				graph_draw_data_->set_polygon_index(i * (vertical_separation_ * 2 - 1) + j * 2 + 1, poly2);
			}
			//中心はポリゴンを1枚だけ張る
			private_::graph_draw_data_2d::polygon_index_type poly;
			poly[0] = i * (vertical_separation_ + 1) + (vertical_separation_ - 1);
			poly[1] = i * (vertical_separation_ + 1) + (vertical_separation_ - 1) + 1;
			poly[2] = (i + 1) * (vertical_separation_ + 1) + (vertical_separation_ - 1);
			graph_draw_data_->set_polygon_index(i * (vertical_separation_ * 2 - 1) + (vertical_separation_ - 1) * 2,
			                                    poly);
		}
	}

	void c_draw_planet_2d::update_polygon_roundly() const {
		//今のところ拡大度は考慮していない
		const vector_2dd center_pos = get_draw_center_position();
		const double rotation = get_draw_rotation_red();
		//頂点座標の算出
		for (unsigned int i = 0; i < horizontal_separation_ + 1; ++i) {
			const double angle_rad = 2.0 * math::pi / horizontal_separation_ * i; //現在の惑星頂点の位置角度
			const double height = planet_component_->get_height_by_rad(angle_rad); //現在の惑星頂点の位置角度の高さ
			vector_2dd interface_vec(std::cos(angle_rad + rotation), std::sin(angle_rad + rotation));
			//中心から地表の現在角度へのベクトル
			interface_vec *= height;
			//中心以外の頂点座標を求める
			for (unsigned int j = 0; j < vertical_separation_; ++j) {
				const double dis_ratio = 1.0f - static_cast<double>(j) / vertical_separation_;
				graph_draw_data_->set_vertex_position(i * (vertical_separation_ + 1) + j,
				                                      static_cast<vector_2df>(center_pos + interface_vec * dis_ratio));
			}
			//中心の頂点座標を求める
			graph_draw_data_->set_vertex_position(i * (vertical_separation_ + 1) + vertical_separation_,
			                                      static_cast<vector_2df>(center_pos));
		}
		//頂点色設定
		for (size_t i = 0; i < graph_draw_data_->vertex_count(); ++i) {
			graph_draw_data_->set_vertex_color(i, color());
		}
	}

	void c_draw_planet_2d::set_polygon_plainly() const {
		//頂点とインデックスのサイズ調整
		graph_draw_data_->set_vertex_count(horizontal_separation_ + 1);
		graph_draw_data_->set_polygon_count(horizontal_separation_);
		//画像と頂点の設定
		for (unsigned int i = 0; i < horizontal_separation_; ++i) {
			//水平方向は座標系正回りにセットしていく
			const double rad = math::pi * 2 / horizontal_separation_ * i;
			//テクスチャ座標のセット
			vector_2df uv;
			uv.x = 0.5f + static_cast<float>(std::cos(rad)) * 0.5f;
			uv.y = 0.5f + static_cast<float>(std::sin(rad)) * 0.5f;
			graph_draw_data_->set_vertex_uv(i, uv);
		}
		graph_draw_data_->set_vertex_uv(horizontal_separation_, vector_2df(0.5f, 0.5f)); //最後の頂点は中心を指す
		//インデックスの設定
		for (unsigned int i = 0; i < horizontal_separation_; ++i) {
			//ポリゴンを1枚ずつ張る
			private_::graph_draw_data_2d::polygon_index_type poly1;
			poly1[0] = i;
			poly1[1] = (i + 1) % horizontal_separation_;
			poly1[2] = horizontal_separation_;
			graph_draw_data_->set_polygon_index(i, poly1);
		}
	}

	void c_draw_planet_2d::update_polygon_plainly() const {
		//今のところ拡大度は考慮していない
		const vector_2dd center_pos = get_draw_center_position();
		const double rotation = get_draw_rotation_red();
		//頂点座標の算出
		for (unsigned int i = 0; i < horizontal_separation_; ++i) {
			const double angle_rad = 2.0 * math::pi / horizontal_separation_ * i; //現在の惑星頂点の位置角度
			const double height = planet_component_->get_height_by_rad(angle_rad); //現在の惑星頂点の位置角度の高さ
			vector_2dd interface_vec(std::cos(angle_rad + rotation), std::sin(angle_rad + rotation));
			//中心から地表の現在角度へのベクトル
			interface_vec *= height;
			graph_draw_data_->set_vertex_position(i, static_cast<vector_2df>(center_pos + interface_vec));
		}
		//中心頂点の設定
		graph_draw_data_->set_vertex_position(horizontal_separation_, static_cast<vector_2df>(center_pos));
		//頂点色設定
		for (size_t i = 0; i < graph_draw_data_->vertex_count(); ++i) {
			graph_draw_data_->set_vertex_color(i, color());
		}
	}

	bool c_draw_planet_2d::set_graph_resource(const std::string &resource_id) {
		if (const auto res = game::instance().resource_manager()->get_resource_by_id<r_graph>(resource_id); res ==
			nullptr) {
			PE_LOG_ERROR("リソースの取得に失敗しました。(ResourceID: ", resource_id, ")");
			return false;
		} else {
			graph_draw_data_->set_graph_resource(res);
			set_polygon_flag_ = true;
			return true;
		}
	}

	c_draw_planet_2d &c_draw_planet_2d::horizontal_separation(const unsigned int sep) {
		horizontal_separation_ = sep;
		set_polygon_flag_ = true;
		return *this;
	}

	c_draw_planet_2d &c_draw_planet_2d::vertical_separation(const unsigned int sep) {
		vertical_separation_ = sep;
		set_polygon_flag_ = true;
		return *this;
	}

	c_draw_planet_2d &c_draw_planet_2d::set_texture_mapping_mode(const texture_mapping_mode tmm) {
		tex_map_mode_ = tmm;
		switch (tex_map_mode_) {
			case texture_mapping_mode::round:
				polygon_setter_ = &c_draw_planet_2d::set_polygon_roundly;
				polygon_updater_ = &c_draw_planet_2d::update_polygon_roundly;
				break;
			case texture_mapping_mode::plain:
				polygon_setter_ = &c_draw_planet_2d::set_polygon_plainly;
				polygon_updater_ = &c_draw_planet_2d::update_polygon_plainly;
				break;
		}
		set_polygon_flag_ = true;
		return *this;
	}

	void c_draw_planet_2d::set_polygon() { (this->*polygon_setter_)(); }

	void c_draw_planet_2d::update_polygon() { (this->*polygon_updater_)(); }

	c_draw_planet_2d &c_draw_planet_2d::graph_resource_id(const std::string &res_id) {
		set_graph_resource(res_id);
		return *this;
	}
}
