#include "..\core\collision_detect_functions.hpp"

#include "CStraightLineCollider2D.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CStraightLineCollider2D) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CStraightLineCollider2D, length)
			.shallow_copy_target(&CStraightLineCollider2D::length_);
	}

	bool CStraightLineCollider2D::collide_with(CCircleCollider2D &circle_collider) {
		return private_::collision_detect_functions::collision_detect_circle_and_straight_line(circle_collider, *this);
	}
}
