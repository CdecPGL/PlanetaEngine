#include "planeta/core/CollisionDetectFunctions.hpp"
#include "planeta/buildin/CCircleCollider2D.hpp"
#include "planeta/buildin/CStraightLineCollider2D.hpp"

namespace plnt {
	namespace private_ {
		namespace collision_detect_functions {

			bool CollisionDetectCircleAndCirecle(CCircleCollider2D& circle_collider0, CCircleCollider2D& circle_collider1)
			{
				auto pos0 = circle_collider0.GetCollisionGlobalCenterPosition();
				auto pos1 = circle_collider1.GetCollisionGlobalCenterPosition();
				double circle_radius0 = circle_collider0.radius()*circle_collider0.GetCollisionScale();
				double circle_radius1 = circle_collider1.radius()*circle_collider1.GetCollisionScale();
				if ((pos1 - pos0).length_square() <= std::pow(circle_radius0 + circle_radius1, 2)) {
					return true;
				}
				else { return false; }
			}

			bool CollisionDetectCircleAndStraightLine(CCircleCollider2D& circle_collider, CStraightLineCollider2D& straightline_collider)
			{
				double circle_radius = circle_collider.radius()*circle_collider.GetCollisionScale();
				auto circle_pos = circle_collider.GetCollisionGlobalCenterPosition();
				auto line_start_pos = straightline_collider.GetCollisionGlobalCenterPosition();
				auto line_vector = Vector2Dd(std::cos(straightline_collider.GetCollisionGlobalRotationRad()), std::sin(straightline_collider.GetCollisionGlobalRotationRad()))*straightline_collider.GetCollisionScale()*straightline_collider.length();
				auto line_start_to_circle_center = circle_pos - line_start_pos;
				//円と線分の距離を求める
				double distance = std::abs(Cross(line_vector, line_start_to_circle_center) / line_vector.length());
				if (distance > circle_radius) { return false; }
				else {
					//円が線分の両端外側にあるか
					double line_start_to_circle_center_dot_line_vector = Dot(line_start_to_circle_center, line_vector);
					Vector2Dd line_end_to_circle_center = line_start_to_circle_center - line_vector;
					if (line_start_to_circle_center_dot_line_vector*Dot(line_end_to_circle_center, line_vector) <= 0) { //線分内にある
						return true;
					}
					else { //線分外にある
						if (line_start_to_circle_center_dot_line_vector > 0) { //線分の終点側にある
							if (line_end_to_circle_center.length() <= circle_radius) { return true; }
							else { return false; }
						}
						else { //線分の始点側にある
							if (line_start_to_circle_center.length() <= circle_radius) { return true; }
							else { return false; }
						}
					}
				}
			}

		}
	}
}