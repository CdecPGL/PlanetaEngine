#pragma once

#include <vector>
#include <array>
#include "CDraw2D.h"

namespace planeta {
	class CDrawPolygon2D final : public CDraw2D {
	public:
		void SetNumberOfVertexes(size_t n);
		/*指定インデックスの座標を設定*/
		bool SetVertexPosition(size_t idx, const Vector2Dd& pos);
		/*頂点を設定*/
		void SetVertexes(const std::vector<Vector2Dd>& vs);
	private:
		std::vector<Vector2Dd> points_;
		std::vector<Vector2Dd> vertexes_;
		std::vector<std::array<int, 3>> indexes_;
		void UpdateVertex_();
		void UpdatePorygonIndex_();
		/*描画処理*/
		void DrawProc(ScreenDrawer2D& drawer)override;
	};
	PE_REGISTER_GAMEOBJECTCOMPONENT(CDrawPolygon2D);
}