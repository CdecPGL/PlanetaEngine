#include "CStraightLineCollider2D.h"
#include "CollisionDetectFunctions.h"

namespace planeta {
	bool CStraightLineCollider2D::CollideWith(CCircleCollider2D& circle_collider) {
		return core::collision_detect_functions::CollisionDetectCircleAndStraightLine(circle_collider, *this);
	}
}