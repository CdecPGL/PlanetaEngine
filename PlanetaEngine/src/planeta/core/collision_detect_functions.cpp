#include "collision_detect_functions.hpp"
#include "planeta/buildin/CCircleCollider2D.hpp"
#include "planeta/buildin/CStraightLineCollider2D.hpp"

namespace plnt::private_::collision_detect_functions {
	bool collision_detect_circle_and_circle(const CCircleCollider2D &circle_collider0,
	                                        const CCircleCollider2D &circle_collider1) {
		const auto pos0 = circle_collider0.GetCollisionGlobalCenterPosition();
		const auto pos1 = circle_collider1.GetCollisionGlobalCenterPosition();
		const double circle_radius0 = circle_collider0.radius() * circle_collider0.GetCollisionScale();
		if (const double circle_radius1 = circle_collider1.radius() * circle_collider1.GetCollisionScale(); (pos1 -
				pos0).length_square() <=
			std::pow(circle_radius0 + circle_radius1, 2)) { return true; }
		return false;
	}

	bool collision_detect_circle_and_straight_line(const CCircleCollider2D &circle_collider,
	                                               const CStraightLineCollider2D &straight_line_collider) {
		const double circle_radius = circle_collider.radius() * circle_collider.GetCollisionScale();
		const auto circle_pos = circle_collider.GetCollisionGlobalCenterPosition();
		const auto line_start_pos = straight_line_collider.GetCollisionGlobalCenterPosition();
		const auto line_vector = Vector2Dd(std::cos(straight_line_collider.GetCollisionGlobalRotationRad()),
		                                   std::sin(straight_line_collider.GetCollisionGlobalRotationRad())) *
			straight_line_collider.GetCollisionScale() * straight_line_collider.length();
		const auto line_start_to_circle_center = circle_pos - line_start_pos;
		//円と線分の距離を求める
		if (const double distance = std::abs(Cross(line_vector, line_start_to_circle_center) / line_vector.length());
			distance > circle_radius) { return false; }
		//円が線分の両端外側にあるか
		const double line_start_to_circle_center_dot_line_vector = Dot(line_start_to_circle_center, line_vector);
		if (const Vector2Dd line_end_to_circle_center = line_start_to_circle_center - line_vector;
			line_start_to_circle_center_dot_line_vector * Dot(line_end_to_circle_center, line_vector) <=
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
