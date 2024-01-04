#pragma once

namespace plnt {
	class c_circle_collider_2d;
	class c_straight_line_collider_2d;

	namespace private_::collision_detect_functions {
		bool collision_detect_circle_and_circle(const c_circle_collider_2d &circle_collider0,
		                                        const c_circle_collider_2d &circle_collider1);
		bool collision_detect_circle_and_straight_line(const c_circle_collider_2d &circle_collider,
		                                               const c_straight_line_collider_2d &straight_line_collider);
	}
}
