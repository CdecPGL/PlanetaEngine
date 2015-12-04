#include "ScreenDrawer2D.h"

#include "DxLib.h"
#include "DXUtility.h"
#include "DXGraphDrawData.h"
#include "GraphDrawData.h"
#include "Color.h"
#include <cassert>

namespace planeta_engine {
	namespace core {
		void ScreenDrawer2D::DrawWire(const std::vector<Vector2D<double>>& positions, double width, const Color& color) {
			using namespace utility::dx;
			VECTOR v0, v1;
			unsigned int dxc = PEColorToDXColorHandle(color);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, color.a());
			for (int i = 0; i < (int)positions.size() - 1; ++i) {
				v0 = PEVector2DToDXVECTOR(positions[i]);
				v1 = PEVector2DToDXVECTOR(positions[i + 1]);
				DrawLine3D(v0, v1, dxc);
			}
		}

		void ScreenDrawer2D::DrawPolygon(const std::vector<Vector2D<double>>& positions, const std::vector<std::array<int, 3>>& indexes, const Color& color) {
			using namespace utility::dx;
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
				//�C���f�b�N�X�ԍ����͈͊O���w���Ă���
				assert(false);
			}
		}

		void ScreenDrawer2D::DrawGraph(const GraphDrawData& graph_draw_data) {
			//�摜�`��f�[�^�������ȏꍇ�͕`�悵�Ȃ�
			if (!graph_draw_data.is_valid()) { return; }
			const DXGraphDrawData& dxgdd = graph_draw_data.GetDXData();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawPolygonIndexed3D(dxgdd.vertexes.get(), (int)dxgdd.vertex_count, dxgdd.indexes.get(), (int)dxgdd.polygon_count, dxgdd.graph_handle, true);
		}
	}
}