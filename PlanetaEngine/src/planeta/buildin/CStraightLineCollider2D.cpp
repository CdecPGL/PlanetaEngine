#include "../core/collision_detect_functions.hpp"

#include "CStraightLineCollider2D.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_straight_line_collider_2d) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_straight_line_collider_2d, length)
			.shallow_copy_target(&c_straight_line_collider_2d::length_);
	}

	bool c_straight_line_collider_2d::collide_with(c_circle_collider_2d &circle_collider) {
		return private_::collision_detect_functions::collision_detect_circle_and_straight_line(circle_collider, *this);
	}
}
