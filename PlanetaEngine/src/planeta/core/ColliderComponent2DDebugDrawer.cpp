#include "ColliderComponent2DDebugDrawer.hpp"
#include "IDebugManager.hpp"

#include "planeta/buildin/CCircleCollider2D.hpp"
#include "planeta/buildin/CStraightLineCollider2D.hpp"
#include "planeta/buildin/CPolygonCollider2D.hpp"


namespace plnt {
	namespace private_ {

		ColliderComponent2DDebugDrawer::ColliderComponent2DDebugDrawer(IDebugDrawer& dd) :debug_drawer_(dd) {}

		bool ColliderComponent2DDebugDrawer::CollideWith(CCircleCollider2D& circle_collider) {
			auto pos = circle_collider.GetCollisionGlobalCenterPosition();
			auto radius = circle_collider.radius() * circle_collider.GetCollisionScale();
			debug_drawer_.DrawCircle(static_cast<Vector2Df>(pos), static_cast<float>(radius), Color::Red(), false);
			return false;
		}

		bool ColliderComponent2DDebugDrawer::CollideWith(CStraightLineCollider2D& straight_line_collider) {
			throw std::logic_error("The method or operation is not implemented.");
		}

		bool ColliderComponent2DDebugDrawer::CollideWith(CPolygonCollider2D& polygon_collider) {
			throw std::logic_error("The method or operation is not implemented.");
		}

	}
}