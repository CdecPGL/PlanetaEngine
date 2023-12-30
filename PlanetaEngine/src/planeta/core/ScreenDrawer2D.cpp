#include <cassert>

#include "DxLib.h"

#include "../dxlib/dx_utility.hpp"
#include "../dxlib/dx_graph_draw_data.hpp"

#include "ScreenDrawer2D.hpp"
#include "graph_draw_data_2d.hpp"
#include "color.hpp"
#include "Screen.hpp"

namespace plnt {
	using namespace dxlib;

	void screen_drawer_2d::draw_wire(const std::vector<Vector2Df> &positions, double width, const color &color) const {
		screen_.reserve_draw([positions,width, color] {
			const unsigned int dxc = pe_color_to_dx_color_handle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			for (int i = 0; i < static_cast<int>(positions.size()) - 1; ++i) {
				const VECTOR v0 = pe_vector_2d_to_dx_vector(positions[i]);
				const VECTOR v1 = pe_vector_2d_to_dx_vector(positions[i + 1]);
				DrawLine3D(v0, v1, dxc);
			}
		});
	}

	void screen_drawer_2d::draw_polygon(const std::vector<Vector2Df> &positions,
	                                  const std::vector<std::array<int, 3>> &indexes, const plnt::color &color) const {
		screen_.reserve_draw([positions,indexes,color] {
			const auto dxc = pe_color_to_dx_color_handle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			try {
				for (auto index : indexes) {
					const VECTOR v0 = pe_vector_2d_to_dx_vector(positions.at(index[0]));
					const VECTOR v1 = pe_vector_2d_to_dx_vector(positions.at(index[1]));
					const VECTOR v2 = pe_vector_2d_to_dx_vector(positions.at(index[2]));
					DrawTriangle3D(v0, v1, v2, dxc, true);
				}
			} catch (std::out_of_range &) {
				//インデックス番号が範囲外を指していた
				assert(false);
			}
		});
	}

	void screen_drawer_2d::draw_circle(const Vector2Df &position, const float radius, const color &color) const {
		const size_t separation = static_cast<size_t>(2.0f * sqrt(radius) * math::pi_f) + 1;
		std::vector<Vector2Df> poses{separation + 1};
		for (size_t i = 0; i < separation; ++i) {
			poses[i] = position + GetUnitVectorByRadian<float>(math::pi * 2 / static_cast<double>(separation * i)) *
				radius;
		}
		poses[separation] = poses[0];
		draw_wire(poses, 1, color);
	}

	void screen_drawer_2d::draw_graph(const std::shared_ptr<private_::graph_draw_data_2d> &graph_draw_data) const {
		screen_.reserve_draw([graph_draw_data] {
			//画像描画データが無効な場合は描画しない
			if (!graph_draw_data->is_valid()) { return; }
			const auto &[vertexes, vertex_count, indexes, polygon_count, graph_handle] = graph_draw_data->get_dx_data();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawPolygonIndexed3D(vertexes.get(), static_cast<int>(vertex_count), indexes.get(),
			                     static_cast<int>(polygon_count), graph_handle, true);
		});
	}
}
