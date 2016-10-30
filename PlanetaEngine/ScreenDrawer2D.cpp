#include "ScreenDrawer2D.h"

#include "DxLib.h"
#include "DXUtility.h"
#include "DXGraphDrawData.h"
#include "GraphDrawData2D.h"
#include "Color.h"
#include "Screen.h"
#include <cassert>

namespace planeta {
	using namespace private_;
	void ScreenDrawer2D::DrawWire(const std::vector<Vector2Df>& positions, double width, const planeta::Color& color) {
		screen_.ReserveDraw([positions,width,color]() {
			using namespace util::dx;
			VECTOR v0, v1;
			unsigned int dxc = PEColorToDXColorHandle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			for (int i = 0; i < (int)positions.size() - 1; ++i) {
				v0 = PEVector2DToDXVECTOR(positions[i]);
				v1 = PEVector2DToDXVECTOR(positions[i + 1]);
				DrawLine3D(v0, v1, dxc);
			}
		});
	}

	void ScreenDrawer2D::DrawPolygon(const std::vector<Vector2Df>& positions, const std::vector<std::array<int, 3>>& indexes, const planeta::Color& color) {
		screen_.ReserveDraw([positions,indexes,color]() {
			using namespace util::dx;
			VECTOR v0, v1, v2;
			int dxc = PEColorToDXColorHandle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			try {
				for (size_t i = 0; i < indexes.size(); ++i) {
					v0 = PEVector2DToDXVECTOR(positions.at(indexes[i][0]));
					v1 = PEVector2DToDXVECTOR(positions.at(indexes[i][1]));
					v2 = PEVector2DToDXVECTOR(positions.at(indexes[i][2]));
					DrawTriangle3D(v0, v1, v2, dxc, true);
				}
			} catch (std::out_of_range&) {
				//インデックス番号が範囲外を指していた
				assert(false);
			}
		});
	}

	void ScreenDrawer2D::DrawCircle(const Vector2Df position, float radius, const Color& color) {
		size_t separation = static_cast<size_t>(2* sqrt(radius)*math::PI) + 1;
		std::vector<Vector2Df> poses{ separation + 1};
		for (size_t i = 0; i < separation; ++i) {
			poses[i] = position + GetUnitVectorByRadian<float>(math::PI * 2 / separation * i) * radius;
		}
		poses[separation] = poses[0];
		DrawWire(poses, 1, color);
	}

	void ScreenDrawer2D::DrawGraph(const std::shared_ptr<private_::GraphDrawData2D>& graph_draw_data) {
		screen_.ReserveDraw([graph_draw_data]() {
			//画像描画データが無効な場合は描画しない
			if (!graph_draw_data->is_valid()) { return; }
			const DXGraphDrawData& dxgdd = graph_draw_data->GetDXData();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawPolygonIndexed3D(dxgdd.vertexes.get(), (int)dxgdd.vertex_count, dxgdd.indexes.get(), (int)dxgdd.polygon_count, dxgdd.graph_handle, true);
		});
	}
}