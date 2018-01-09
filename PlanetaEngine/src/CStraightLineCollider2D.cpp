#include "CStraightLineCollider2D.hpp"
#include "CollisionDetectFunctions.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CStraightLineCollider2D) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CStraightLineCollider2D, length)
			.ShallowCopyTarget(&CStraightLineCollider2D::length_);
	}

	bool CStraightLineCollider2D::CollideWith(CCircleCollider2D& circle_collider) {
		return private_::collision_detect_functions::CollisionDetectCircleAndStraightLine(circle_collider, *this);
	}
}