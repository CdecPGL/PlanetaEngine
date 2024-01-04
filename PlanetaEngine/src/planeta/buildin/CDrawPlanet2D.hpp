#pragma once

#include "CDraw2D.hpp"

namespace plnt {
	namespace private_ {
		class graph_draw_data_2d;
	}

	/*! 惑星描画コンポーネント。
	
		@note DrawPlanetComponentのseparationはPlanetComponentのseparationとは異なる
	*/
	class c_planet;

	class c_draw_planet_2d final : public c_draw_2d {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_draw_planet_2d);

	public:
		using super = c_draw_2d;
		c_draw_planet_2d();
		~c_draw_planet_2d() override;
		/*! 画像リソースを設定する*/
		bool set_graph_resource(const std::string &resource_id);

		/*! 画像のマッピングモードを表す列挙体*/
		enum class texture_mapping_mode { round, plain };

		/*アクセサ*/
		/*! 画像リソースをIDで設定する*/
		c_draw_planet_2d &graph_resource_id(const std::string &res_id);
		/*! 水平方向の分割数を取得する*/
		[[nodiscard]] unsigned int horizontal_separation() const { return horizontal_separation_; }
		/*! 水平方向の分割数を設定する*/
		c_draw_planet_2d &horizontal_separation(unsigned int sep);
		/*! 垂直方向の分割数を取得する*/
		[[nodiscard]] unsigned int vertical_separation() const { return vertical_separation_; }
		/*! 垂直方向の分割数を設定する*/
		c_draw_planet_2d &vertical_separation(unsigned int sep);
		/*! 画像のマッピングモードを取得する*/
		[[nodiscard]] texture_mapping_mode get_texture_mapping_mode() const { return tex_map_mode_; }
		/*! 画像のマッピングモードを設定する*/
		c_draw_planet_2d &set_texture_mapping_mode(texture_mapping_mode tmm);

	private:
		/*水平分割数*/
		unsigned int horizontal_separation_;
		/*垂直分割数*/
		unsigned int vertical_separation_;
		/*ポリゴンの設定が必要か*/
		bool set_polygon_flag_ = false;
		/*画像描画データ*/
		std::shared_ptr<private_::graph_draw_data_2d> graph_draw_data_;
		/*テクスチャマッピングモード*/
		texture_mapping_mode tex_map_mode_;
		bool get_other_components_proc(const go_component_getter &com_getter) override;
		void on_initialized() override;
		void on_finalized() noexcept override;
		non_owing_pointer<c_planet> planet_component_;
		/*ポリゴンセット*/
		void (c_draw_planet_2d::*polygon_setter_)() const;
		void set_polygon();
		void set_polygon_roundly() const;
		void set_polygon_plainly() const;
		/*ポリゴン情報更新*/
                void (c_draw_planet_2d::*polygon_updater_) () const;
		void update_polygon();
		void update_polygon_roundly() const;
		void update_polygon_plainly() const;
		/*描画処理*/
		void draw_proc(screen_drawer_2d &drawer) override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_draw_planet_2d);

	//ReflectionシステムのPtree読み込みを有効にするための定義
	template <>
	struct reflection::reflective_ptree_converter_impl<c_draw_planet_2d::texture_mapping_mode> {
		void operator()(c_draw_planet_2d::texture_mapping_mode &dst, const boost::property_tree::ptree &src) const {
			try {
				if (const auto str = src.get_value<std::string>(); str == "Round") {
					dst = c_draw_planet_2d::texture_mapping_mode::round;
				} else if (str == "Plain") { dst = c_draw_planet_2d::texture_mapping_mode::plain; } else {
					throw reflection_error(util::convert_and_connect_to_string(
						"\"", src.get_value<std::string>(), "\"は\"",
						typeid(c_draw_planet_2d::texture_mapping_mode).name(), "\"のメンバーではありません。"));
				}
			} catch (boost::property_tree::ptree_bad_data &e) { throw reflection_error(e.what()); }
		}
	};
}
