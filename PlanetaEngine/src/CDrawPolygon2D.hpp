#pragma once

#include <vector>
#include <array>
#include "CDraw2D.hpp"

namespace plnt {
	/*! 平面多角形描画コンポーネント*/
	class CDrawPolygon2D final : public CDraw2D {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDrawPolygon2D);
	public:
		using Super = CDraw2D;
		/*! 腸点数を設定する*/
		void SetNumberOfVertexes(size_t n);
		/*! 指定インデックスの座標を設定*/
		bool SetVertexPosition(size_t idx, const Vector2Dd& pos);
		/*! 全ての頂点を設定する*/
		void vertexes(const std::vector<Vector2Dd>& vs);
	private:
		std::vector<Vector2Df> points_;
		std::vector<Vector2Df> vertexes_;
		std::vector<std::array<int, 3>> indexes_;
		void UpdateVertex_();
		void UpdatePorygonIndex_();
		/*描画処理*/
		void DrawProc(ScreenDrawer2D& drawer)override;
	};
	PE_GAMEOBJECTCOMPONENT_CLASS(CDrawPolygon2D);
}