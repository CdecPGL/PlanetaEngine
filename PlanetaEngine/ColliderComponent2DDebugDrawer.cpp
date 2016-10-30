#include "ColliderComponent2DDebugDrawer.h"
#include "IDebugManager.h"

#include "CCircleCollider2D.h"
#include "CStraightLineCollider2D.h"
#include "CPolygonCollider2D.h"


namespace planeta {
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