#include "CCircleCollider2D.h"
#include "CollisionDetectFunctions.h"

namespace planeta {

	bool CCircleCollider2D::CollideWith(CCircleCollider2D& circle_collider) {
		return core::collision_detect_functions::CollisionDetectCircleAndCirecle(circle_collider, *this);
	}

	bool CCircleCollider2D::CollideWith(CStraightLineCollider2D& straight_line_collider) {
		return core::collision_detect_functions::CollisionDetectCircleAndStraightLine(*this, straight_line_collider);
	}
}