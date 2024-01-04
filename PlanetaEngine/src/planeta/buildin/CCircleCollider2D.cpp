#include "../core/collision_detect_functions.hpp"
#include "CCircleCollider2D.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_circle_collider_2d) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_circle_collider_2d, radius)
			.shallow_copy_target(&c_circle_collider_2d::radius_);
	}

	bool c_circle_collider_2d::collide_with(c_circle_collider_2d &circle_collider) {
		return private_::collision_detect_functions::collision_detect_circle_and_circle(circle_collider, *this);
	}

	bool c_circle_collider_2d::collide_with(c_straight_line_collider_2d &straight_line_collider) {
		return private_::collision_detect_functions::collision_detect_circle_and_straight_line(
			*this, straight_line_collider);
	}
}
