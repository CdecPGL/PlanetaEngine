#include "DrawPolygonComponent.h"
#include "DrawManager.h"
#include "IGameObjectAccessor.h"
#include "TransformComponent.h"
#include "Matrix.h"

namespace planeta_engine {
	namespace components {

		void DrawPolygonComponent::Draw()
		{
			UpdateVertex_();
			core::DrawManager::instance().DrawPolygon(vertexes_, indexes_, color());
		}

		void DrawPolygonComponent::SetNumberOfVertexes(size_t n)
		{
			points_.resize(n);
			vertexes_.resize(n);
			UpdatePorygonIndex_();
		}

		bool DrawPolygonComponent::SetVertexPosition(size_t idx, const math::Vector2Dd& pos)
		{
			if (idx >= points_.size()) { return false; }
			else {
				points_[idx] = pos;
				return true;
			}
		}

		void DrawPolygonComponent::SetVertexes(const std::vector<math::Vector2Dd>& vs)
		{
			points_ = vs;
			vertexes_.resize(points_.size());
			UpdatePorygonIndex_();
		}

		void DrawPolygonComponent::UpdatePorygonIndex_()
		{
			indexes_.clear();
			if (points_.size() < 3) { return; }
			for (size_t i = 0; i < points_.size() - 2; ++i) {
				indexes_.push_back({ 0,(int)i + 1,(int)i + 2 });
			}
		}

		void DrawPolygonComponent::UpdateVertex_()
		{
			components::TransformComponent& transform = game_object().transform();
			for (size_t i = 0; i < points_.size(); ++i) {
				math::Vector2Dd relative_pos0(points_[i].x*scale().x, points_[i].y*scale().y); //描画コンポーネントの拡大度を適用
				relative_pos0 = math::RotationalTransformation(rotation_rad(), relative_pos0); //描画コンポーネントの回転度を適用
				auto relative_pos1 = math::Vector2Dd((position() + relative_pos0).x*transform.global_scale().x, (position() + relative_pos0).y*transform.global_scale().y); //トランスフォームの拡大度を適用
				auto pos = transform.global_position() + math::RotationalTransformation(transform.global_rotation_rad(), relative_pos1); //トランスフォームの回転度を適用
				vertexes_[i] = pos;
			}
		}

	}
}