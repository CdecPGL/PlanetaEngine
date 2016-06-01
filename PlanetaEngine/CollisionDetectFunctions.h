#pragma once

#include <memory>

namespace planeta {
		class CCircleCollider2D;
		class CStraightLineCollider2D;
	namespace core {
		namespace collision_detect_functions {
			bool CollisionDetectCircleAndCirecle(CCircleCollider2D& circle_collider0, CCircleCollider2D& circle_collider1);
			bool CollisionDetectCircleAndStraightLine(CCircleCollider2D& circle_collider, CStraightLineCollider2D& straightline_collider);
		}
	}
}
