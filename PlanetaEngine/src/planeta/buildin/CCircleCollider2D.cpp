#include "..\core\collision_detect_functions.hpp"

#include "CCircleCollider2D.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CCircleCollider2D) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CCircleCollider2D, radius)
			.shallow_copy_target(&CCircleCollider2D::_radius);
	}

	bool CCircleCollider2D::collide_with(CCircleCollider2D &circle_collider) {
		return private_::collision_detect_functions::collision_detect_circle_and_circle(circle_collider, *this);
	}

	bool CCircleCollider2D::collide_with(CStraightLineCollider2D &straight_line_collider) {
		return private_::collision_detect_functions::collision_detect_circle_and_straight_line(
			*this, straight_line_collider);
	}
}
