#pragma once

#include <memory>
#include <array>
#include <vector>

#include "vertex_2d.hpp"

namespace plnt {
	class RGraph;

	namespace dxlib {
		struct dx_graph_draw_data;
	}

	namespace private_ {
		class graph_draw_data_2d {
		public:
			using polygon_index_type = std::array<size_t, 3>;

			graph_draw_data_2d();
			graph_draw_data_2d(const graph_draw_data_2d &obj);
			graph_draw_data_2d(graph_draw_data_2d &&obj) noexcept = default;
			~graph_draw_data_2d();
			graph_draw_data_2d &operator=(const graph_draw_data_2d &obj);
			graph_draw_data_2d &operator=(graph_draw_data_2d &&obj) noexcept = default;

			[[nodiscard]] const dxlib::dx_graph_draw_data &get_dx_data() const { return *dx_data_; }
			/*画像リソースをセットする*/
			void set_graph_resource(const std::shared_ptr<RGraph> &g_res);
			void set_vertex_count(size_t c); //頂点数を設定する
			void set_polygon_count(size_t c) const; //ポリゴン数を設定する
			bool set_vertex(size_t idx, const util::vertex_2d &vtx); //頂点を設定する
			// NOLINTNEXTLINE(modernize-use-nodiscard)
			bool set_vertex_position(size_t idx, const vector_2df &pos) const; //頂点の位置を設定する
			bool set_vertex_uv(size_t idx, const vector_2df &uv); //頂点のUV座標を設定する
			// NOLINTNEXTLINE(modernize-use-nodiscard)
			bool set_vertex_color(size_t idx, const color &color) const; //頂点の色を設定する
			// NOLINTNEXTLINE(modernize-use-nodiscard)
			bool set_polygon_index(size_t idx, const std::array<size_t, 3> &p_idx) const; //ポリゴンの頂点インデックスを設定する
			void set_vertexes(const std::vector<util::vertex_2d> &vertexes); //頂点を全て設定
			void set_polygon_indexes(const std::vector<std::array<size_t, 3>> &polygon_indexes) const; //ポリゴンインデックスを全て設定
			void set_vertexes_and_polygon_indexes(const std::vector<util::vertex_2d> &vertexes,
			                                      const std::vector<std::array<size_t, 3>> &polygon_indexes);
			//頂点とポリゴンインデックスをまとめて設定する
			[[nodiscard]] bool is_valid() const;
			[[nodiscard]] size_t vertex_count() const;
			[[nodiscard]] size_t polygon_count() const;
			[[nodiscard]] std::shared_ptr<RGraph> graph_resource() const { return graph_resource_; }
		private:
			std::unique_ptr<dxlib::dx_graph_draw_data> dx_data_;
			std::shared_ptr<RGraph> graph_resource_;
			std::vector<vector_2df> vertex_uv_information_buffer_; //頂点UVデータの更新用に、元のUV座標データを保持しておく
		};
	}
}
