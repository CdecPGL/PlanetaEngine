#pragma once

#include <vector>
#include <array>

#include "c_draw_2d.hpp"

namespace plnt {
	/*! 平面多角形描画コンポーネント*/
	class c_draw_polygon_2d final : public c_draw_2d {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_draw_polygon_2d);

	public:
		using super = c_draw_2d;
		/*! 腸点数を設定する*/
		void set_number_of_vertexes(size_t n);
		/*! 指定インデックスの座標を設定*/
		bool set_vertex_position(size_t idx, const vector_2dd &pos);
		/*! 全ての頂点を設定する*/
		void vertexes(const std::vector<vector_2dd> &vs);

	private:
		std::vector<vector_2df> points_;
		std::vector<vector_2df> vertexes_;
		std::vector<std::array<int, 3>> indexes_;
		void update_vertex();
		void update_polygon_index();
		/*描画処理*/
		void draw_proc(screen_drawer_2d &drawer) override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_draw_polygon_2d);
}
