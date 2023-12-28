#pragma once

namespace plnt {
	class CCircleCollider2D;
	class CStraightLineCollider2D;

	namespace private_::collision_detect_functions {
		bool collision_detect_circle_and_circle(const CCircleCollider2D &circle_collider0,
		                                        const CCircleCollider2D &circle_collider1);
		bool collision_detect_circle_and_straight_line(const CCircleCollider2D &circle_collider,
		                                               const CStraightLineCollider2D &straight_line_collider);
	}
}
