#pragma once

#include "planeta/core/rectangle.hpp"

#include "CDrawGUI.hpp"

namespace plnt {
	/*! GUI領域に画像を描画するコンポーネント*/
	class c_draw_graph_gui final : public c_draw_gui {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_draw_graph_gui);

	public:
		using super = c_draw_gui;
		c_draw_graph_gui();
		~c_draw_graph_gui() override;
		/*! 表示する画像のリソースIDを設定する*/
		bool resource_id(const std::string &res_id) const;
		/*! 画像上の描画領域をピクセル単位で取得*/
		[[nodiscard]] const rectangle_i &draw_area() const;
		/*! 画像上の描画領域をピクセル単位で設定*/
		c_draw_graph_gui &draw_area(const rectangle_i &rect);
		/*! 画像を反転させるか取得*/
		[[nodiscard]] bool reverse() const;
		/*! 画像を反転させるか設定*/
		c_draw_graph_gui &reverse(bool rev);

	private:
		class impl;
		std::unique_ptr<impl> impl_;
		void draw_proc(screen_drawer_gui &drawer) override;
		bool get_other_components_proc(const go_component_getter &) override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_draw_graph_gui);
}
