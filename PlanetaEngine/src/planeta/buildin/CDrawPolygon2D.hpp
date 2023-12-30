#pragma once

#include <vector>
#include <array>

#include "CDraw2D.hpp"

namespace plnt {
	/*! 平面多角形描画コンポーネント*/
	class CDrawPolygon2D final : public CDraw2D {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(CDrawPolygon2D);

	public:
		using super = CDraw2D;
		/*! 腸点数を設定する*/
		void SetNumberOfVertexes(size_t n);
		/*! 指定インデックスの座標を設定*/
		bool SetVertexPosition(size_t idx, const vector_2dd &pos);
		/*! 全ての頂点を設定する*/
		void vertexes(const std::vector<vector_2dd> &vs);

	private:
		std::vector<vector_2df> points_;
		std::vector<vector_2df> vertexes_;
		std::vector<std::array<int, 3>> indexes_;
		void UpdateVertex_();
		void UpdatePorygonIndex_();
		/*描画処理*/
		void DrawProc(screen_drawer_2d &drawer) override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(CDrawPolygon2D);
}
