#include "CDrawPolygon2D.h"
#include "ScreenDrawer2D.h"
#include "IGameObjectForComponent.h"
#include "CTransform2D.h"
#include "Matrix2_2.h"

namespace planeta_engine {
	void CDrawPolygon2D::DrawProc(ScreenDrawer2D& drawer) {
		UpdateVertex_();
		drawer.DrawPolygon(vertexes_, indexes_, color());
	}

	void CDrawPolygon2D::SetNumberOfVertexes(size_t n) {
		points_.resize(n);
		vertexes_.resize(n);
		UpdatePorygonIndex_();
	}

	bool CDrawPolygon2D::SetVertexPosition(size_t idx, const Vector2Dd& pos) {
		if (idx >= points_.size()) { return false; } else {
			points_[idx] = pos;
			return true;
		}
	}

	void CDrawPolygon2D::SetVertexes(const std::vector<Vector2Dd>& vs) {
		points_ = vs;
		vertexes_.resize(points_.size());
		UpdatePorygonIndex_();
	}

	void CDrawPolygon2D::UpdatePorygonIndex_() {
		indexes_.clear();
		if (points_.size() < 3) { return; }
		for (size_t i = 0; i < points_.size() - 2; ++i) {
			indexes_.push_back({ 0,(int)i + 1,(int)i + 2 });
		}
	}

	void CDrawPolygon2D::UpdateVertex_() {
		for (size_t i = 0; i < points_.size(); ++i) {
			Vector2Dd relative_pos0(points_[i].x*scale().x, points_[i].y*scale().y); //描画コンポーネントの拡大度を適用
			relative_pos0 = math::RotationalTransformation(rotation_rad(), relative_pos0); //描画コンポーネントの回転度を適用
			auto relative_pos1 = Vector2Dd((position() + relative_pos0).x*transform2d_->scale().x, (position() + relative_pos0).y*transform2d_->scale().y); //トランスフォームの拡大度を適用
			auto pos = transform2d_->global_position() + math::RotationalTransformation(transform2d_->global_rotation_rad(), relative_pos1); //トランスフォームの回転度を適用
			vertexes_[i] = pos;
		}
	}
}