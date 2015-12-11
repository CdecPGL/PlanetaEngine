#pragma once

#include <vector>
#include <array>
#include "DrawComponent.h"

namespace planeta_engine {
	namespace components {
		class DrawPolygonComponent final: public DrawComponent{
		public:
			void SetNumberOfVertexes(size_t n);
			/*指定インデックスの座標を設定*/
			bool SetVertexPosition(size_t idx, const Vector2D<double>& pos);
			/*頂点を設定*/
			void SetVertexes(const std::vector<Vector2D<double>>& vs);
		private:
			std::vector<Vector2D<double>> points_;
			std::vector<Vector2D<double>> vertexes_;
			std::vector<std::array<int, 3>> indexes_;
			void UpdateVertex_();
			void UpdatePorygonIndex_();
			/*描画処理*/
			void DrawProc(ScreenDrawer2D& drawer)override;
		};
	}
}