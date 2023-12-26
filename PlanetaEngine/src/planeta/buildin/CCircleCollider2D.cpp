#include "planeta/core/CollisionDetectFunctions.hpp"

#include "CCircleCollider2D.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CCircleCollider2D) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CCircleCollider2D, radius)
			.ShallowCopyTarget(&CCircleCollider2D::_radius);
	}

	bool CCircleCollider2D::CollideWith(CCircleCollider2D &circle_collider) {
		return private_::collision_detect_functions::CollisionDetectCircleAndCirecle(circle_collider, *this);
	}

	bool CCircleCollider2D::CollideWith(CStraightLineCollider2D &straight_line_collider) {
		return private_::collision_detect_functions::CollisionDetectCircleAndStraightLine(
			*this, straight_line_collider);
	}
}
