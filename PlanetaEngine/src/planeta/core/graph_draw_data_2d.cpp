#include "..\buildin\r_graph.hpp"
#include "../dxlib/dx_graph_draw_data.hpp"
#include "../dxlib/dx_utility.hpp"
#include "graph_draw_data_2d.hpp"

namespace plnt::private_ {
	namespace {
		/*DX形式頂点情報の初期化*/
		void initialize_dxvertex_3d(VERTEX3D &dx_vtx3d) {
			dx_vtx3d.pos = VGet(0, 0, 0);
			dx_vtx3d.norm = VGet(0.0f, 0.0f, -1.0f); //画面側を向くように
			dx_vtx3d.u = 0;
			dx_vtx3d.v = 0;
			dx_vtx3d.dif = GetColorU8(255, 255, 255, 255);
		}

		/*DX形式頂点情報の位置を設定*/
		void set_position_to_dxvertex_3d(VERTEX3D &dx_vtx3d, const vector_2df &position) {
			using namespace dxlib;
			dx_vtx3d.pos = pe_vector_2d_to_dx_vector(position);
		}

		/*DX形式頂点情報のUV座標を設定*/
		void set_uv_to_dxvertex_3d(VERTEX3D &dx_vtx3d, const float u, const float v, const vector_2dd &image_area) {
			dx_vtx3d.u = static_cast<float>(static_cast<double>(u) * image_area.x);
			dx_vtx3d.v = static_cast<float>(static_cast<double>(v) * image_area.y);
		}

		/*DX形式頂点情報の色を設定*/
		void set_color_to_dxvertex_3d(VERTEX3D &dx_vtx3d, const color &color) {
			using namespace dxlib;
			dx_vtx3d.dif = pe_color_to_dx_color_u8(color);
			//				vdx[i].spc = pe_color_to_dx_color_u8(vertexes[i].color); //ライティング計算を行わないので使用しない
		}

		/*DX形式頂点情報を設定*/
		void set_pe_vertex_2d_to_dxvertex_3d(VERTEX3D &dx_vtx3d, const util::vertex_2d &pe_vtx2d,
		                                     const vector_2dd &image_area) {
			using namespace dxlib;
			set_position_to_dxvertex_3d(dx_vtx3d, pe_vtx2d.position);
			set_color_to_dxvertex_3d(dx_vtx3d, pe_vtx2d.color);
			set_uv_to_dxvertex_3d(dx_vtx3d, pe_vtx2d.u, pe_vtx2d.v, image_area);
		}

		/*DX形式ポリゴンインデックス情報を設定*/
		void convert_pe_polygon_index_to_dx_index_array(unsigned short *dx_idx_array_ptr, const size_t write_pos_top,
		                                                const std::array<size_t, 3> &pe_p_idx) {
			for (int j = 0; j < 3; ++j) {
				dx_idx_array_ptr[write_pos_top + j] = static_cast<unsigned short>(pe_p_idx[j]);
			}
		}
	}

	graph_draw_data_2d::graph_draw_data_2d() : dx_data_(std::make_unique<dxlib::dx_graph_draw_data>()) { }

	graph_draw_data_2d::graph_draw_data_2d(const graph_draw_data_2d &obj) : dx_data_(
		std::make_unique<dxlib::dx_graph_draw_data>()) { *this = obj; }

	graph_draw_data_2d::~graph_draw_data_2d() = default;

	void graph_draw_data_2d::set_graph_resource(const std::shared_ptr<r_graph> &g_res) {
		graph_resource_ = g_res;
		dx_data_->graph_handle = g_res->get_handle();
		//頂点UV座標の更新
		if (dx_data_->vertexes) {
			for (size_t i = 0; i < dx_data_->vertex_count; ++i) {
				auto &vtx = dx_data_->vertexes[i];
				set_uv_to_dxvertex_3d(vtx, vertex_uv_information_buffer_[i].x, vertex_uv_information_buffer_[i].y,
				                      g_res->image_area());
			}
		}
	}

	void graph_draw_data_2d::set_vertex_count(const size_t c) {
		dx_data_->vertexes = std::make_unique<VERTEX3D[]>(c);
		dx_data_->vertex_count = c;
		for (size_t i = 0; i < c; ++i) { initialize_dxvertex_3d(dx_data_->vertexes[i]); }
		vertex_uv_information_buffer_.resize(c);
	}

	void graph_draw_data_2d::set_polygon_count(const size_t c) const {
		dx_data_->indexes = std::make_unique<unsigned short[]>(c * 3);
		dx_data_->polygon_count = c;
	}

	bool graph_draw_data_2d::set_vertex(const size_t idx, const util::vertex_2d &vtx) {
		if (dx_data_->vertex_count <= idx) { return false; }
		set_pe_vertex_2d_to_dxvertex_3d(dx_data_->vertexes[idx], vtx, graph_resource_->image_area());
		vertex_uv_information_buffer_[idx].x = vtx.u;
		vertex_uv_information_buffer_[idx].y = vtx.v;
		return true;
	}

	bool graph_draw_data_2d::set_vertex_position(const size_t idx, const vector_2df &pos) const {
		if (dx_data_->vertex_count <= idx) { return false; }
		set_position_to_dxvertex_3d(dx_data_->vertexes[idx], pos);
		return true;
	}

	bool graph_draw_data_2d::set_vertex_uv(const size_t idx, const vector_2df &uv) {
		if (dx_data_->vertex_count <= idx) { return false; }
		if (graph_resource_ == nullptr) { return false; }
		set_uv_to_dxvertex_3d(dx_data_->vertexes[idx], uv.x, uv.y, graph_resource_->image_area());
		vertex_uv_information_buffer_[idx] = uv;
		return true;
	}

	bool graph_draw_data_2d::set_vertex_color(const size_t idx, const color &color) const {
		if (dx_data_->vertex_count <= idx) { return false; }
		set_color_to_dxvertex_3d(dx_data_->vertexes[idx], color);
		return true;
	}

	bool graph_draw_data_2d::set_polygon_index(const size_t idx, const std::array<size_t, 3> &p_idx) const {
		if (dx_data_->polygon_count <= idx) { return false; }
		convert_pe_polygon_index_to_dx_index_array(dx_data_->indexes.get(), idx * 3, p_idx);
		return true;
	}

	void graph_draw_data_2d::set_vertexes(const std::vector<util::vertex_2d> &vertexes) {
		using namespace dxlib;
		//頂点情報のDX形式変換
		auto vdx = std::make_unique<VERTEX3D[]>(vertexes.size());
		vertex_uv_information_buffer_.resize(vertexes.size());
		for (size_t i = 0; i < vertexes.size(); ++i) {
			initialize_dxvertex_3d(vdx[i]);
			set_pe_vertex_2d_to_dxvertex_3d(vdx[i], vertexes[i], graph_resource_->image_area());
			vertex_uv_information_buffer_[i].x = vertexes[i].u;
			vertex_uv_information_buffer_[i].y = vertexes[i].v;
		}
		dx_data_->vertexes = std::move(vdx);
		dx_data_->vertex_count = vertexes.size();
	}

	void graph_draw_data_2d::set_polygon_indexes(const std::vector<std::array<size_t, 3>> &polygon_indexes) const {
		//インデックス情報のDX形式変換
		std::unique_ptr<unsigned short[]> idx = std::make_unique<unsigned short[]>(polygon_indexes.size() * 3);
		for (size_t i = 0; i < polygon_indexes.size(); ++i) {
			convert_pe_polygon_index_to_dx_index_array(idx.get(), i * 3, polygon_indexes[i]);
		}
		dx_data_->indexes = std::move(idx);
		dx_data_->polygon_count = polygon_indexes.size();
	}

	void graph_draw_data_2d::set_vertexes_and_polygon_indexes(const std::vector<util::vertex_2d> &vertexes,
	                                                          const std::vector<std::array<size_t, 3>> &
	                                                          polygon_indexes) {
		set_vertexes(vertexes);
		set_polygon_indexes(polygon_indexes);
	}

	bool graph_draw_data_2d::is_valid() const {
		return graph_resource_ != nullptr && dx_data_->indexes != nullptr && dx_data_->vertexes != nullptr;
	}

	size_t graph_draw_data_2d::vertex_count() const { return dx_data_->vertex_count; }

	size_t graph_draw_data_2d::polygon_count() const { return dx_data_->polygon_count; }

	graph_draw_data_2d &graph_draw_data_2d::operator=(const graph_draw_data_2d &obj) {
		if (this == &obj) { return *this; }

		dx_data_->graph_handle = obj.dx_data_->graph_handle;
		dx_data_->vertex_count = obj.dx_data_->vertex_count;
		dx_data_->polygon_count = obj.dx_data_->polygon_count;
		dx_data_->vertexes = std::make_unique<VERTEX3D[]>(dx_data_->vertex_count);
		memcpy_s(dx_data_->vertexes.get(), sizeof(VERTEX3D) * dx_data_->vertex_count, obj.dx_data_->vertexes.get(),
		         sizeof(VERTEX3D) * dx_data_->vertex_count);
		dx_data_->indexes = std::make_unique<unsigned short[]>(dx_data_->polygon_count * 3);
		memcpy_s(dx_data_->indexes.get(), sizeof(unsigned short) * dx_data_->polygon_count * 3,
		         obj.dx_data_->indexes.get(), sizeof(unsigned short) * dx_data_->polygon_count * 3);

		graph_resource_ = obj.graph_resource_;
		vertex_uv_information_buffer_ = obj.vertex_uv_information_buffer_;
		return *this;
	}
}
