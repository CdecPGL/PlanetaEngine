﻿#include "collision_detect_functions.hpp"
#include "..\buildin\c_circle_collider_2d.hpp"
#include "..\buildin\c_straight_line_collider_2d.hpp"

namespace plnt::private_::collision_detect_functions {
	bool collision_detect_circle_and_circle(const c_circle_collider_2d &circle_collider0,
	                                        const c_circle_collider_2d &circle_collider1) {
		const auto pos0 = circle_collider0.get_collision_global_center_position();
		const auto pos1 = circle_collider1.get_collision_global_center_position();
		const double circle_radius0 = circle_collider0.radius() * circle_collider0.get_collision_scale();
		if (const double circle_radius1 = circle_collider1.radius() * circle_collider1.get_collision_scale(); (pos1 -
				pos0).length_square() <=
			std::pow(circle_radius0 + circle_radius1, 2)) { return true; }
		return false;
	}

	bool collision_detect_circle_and_straight_line(const c_circle_collider_2d &circle_collider,
	                                               const c_straight_line_collider_2d &straight_line_collider) {
		const double circle_radius = circle_collider.radius() * circle_collider.get_collision_scale();
		const auto circle_pos = circle_collider.get_collision_global_center_position();
		const auto line_start_pos = straight_line_collider.get_collision_global_center_position();
		const auto line_vector = vector_2dd(std::cos(straight_line_collider.get_collision_global_rotation_rad()),
		                                   std::sin(straight_line_collider.get_collision_global_rotation_rad())) *
			straight_line_collider.get_collision_scale() * straight_line_collider.length();
		const auto line_start_to_circle_center = circle_pos - line_start_pos;
		//円と線分の距離を求める
		if (const double distance = std::abs(cross(line_vector, line_start_to_circle_center) / line_vector.length());
			distance > circle_radius) { return false; }
		//円が線分の両端外側にあるか
		const double line_start_to_circle_center_dot_line_vector = dot(line_start_to_circle_center, line_vector);
		if (const vector_2dd line_end_to_circle_center = line_start_to_circle_center - line_vector;
			line_start_to_circle_center_dot_line_vector * dot(line_end_to_circle_center, line_vector) <=
			0) {
			//線分内にある
			return true;
		} else {
			//線分外にある
			if (line_start_to_circle_center_dot_line_vector > 0) {
				//線分の終点側にある
				if (line_end_to_circle_center.length() <= circle_radius) { return true; }
				return false;
			}
			//線分の始点側にある
			if (line_start_to_circle_center.length() <= circle_radius) { return true; }
			return false;
		}
	}
}
