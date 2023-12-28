#include <cassert>

#include "DxLib.h"

#include "..\dxlib\dx_utility.hpp"
#include "..\dxlib\dx_graph_draw_data.hpp"

#include "ScreenDrawer2D.hpp"
#include "GraphDrawData2D.hpp"
#include "color.hpp"
#include "Screen.hpp"

namespace plnt {
	using namespace dxlib;

	void ScreenDrawer2D::DrawWire(const std::vector<Vector2Df> &positions, double width, const plnt::color &color) {
		screen_.ReserveDraw([positions,width,color]() {
			VECTOR v0, v1;
			unsigned int dxc = pe_color_to_dx_color_handle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			for (int i = 0; i < (int)positions.size() - 1; ++i) {
				v0 = pe_vector_2d_to_dx_vector(positions[i]);
				v1 = pe_vector_2d_to_dx_vector(positions[i + 1]);
				DrawLine3D(v0, v1, dxc);
			}
		});
	}

	void ScreenDrawer2D::DrawPolygon(const std::vector<Vector2Df> &positions,
	                                 const std::vector<std::array<int, 3>> &indexes, const plnt::color &color) {
		screen_.ReserveDraw([positions,indexes,color]() {
			VECTOR v0, v1, v2;
			int dxc = pe_color_to_dx_color_handle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			try {
				for (size_t i = 0; i < indexes.size(); ++i) {
					v0 = pe_vector_2d_to_dx_vector(positions.at(indexes[i][0]));
					v1 = pe_vector_2d_to_dx_vector(positions.at(indexes[i][1]));
					v2 = pe_vector_2d_to_dx_vector(positions.at(indexes[i][2]));
					DrawTriangle3D(v0, v1, v2, dxc, true);
				}
			} catch (std::out_of_range &) {
				//インデックス番号が範囲外を指していた
				assert(false);
			}
		});
	}

	void ScreenDrawer2D::DrawCircle(const Vector2Df position, float radius, const color &color) {
		size_t separation = static_cast<size_t>(2 * sqrt(radius) * math::pi) + 1;
		std::vector<Vector2Df> poses{separation + 1};
		for (size_t i = 0; i < separation; ++i) {
			poses[i] = position + GetUnitVectorByRadian<float>(math::pi * 2 / separation * i) * radius;
		}
		poses[separation] = poses[0];
		DrawWire(poses, 1, color);
	}

	void ScreenDrawer2D::DrawGraph(const std::shared_ptr<private_::GraphDrawData2D> &graph_draw_data) {
		screen_.ReserveDraw([graph_draw_data]() {
			//画像描画データが無効な場合は描画しない
			if (!graph_draw_data->is_valid()) { return; }
			const dx_graph_draw_data &dxgdd = graph_draw_data->GetDXData();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawPolygonIndexed3D(dxgdd.vertexes.get(), (int)dxgdd.vertex_count, dxgdd.indexes.get(),
			                     (int)dxgdd.polygon_count, dxgdd.graph_handle, true);
		});
	}
}
