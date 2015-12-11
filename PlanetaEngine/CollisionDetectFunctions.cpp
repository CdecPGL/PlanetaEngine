#include "CollisionDetectFunctions.h"
#include "CircleColliderComponent.h"
#include "StraightLineColliderComponent.h"

namespace planeta_engine {
	namespace core {
		namespace collision_detect_functions {

			bool CollisionDetectCircleAndCirecle(components::CircleColliderComponent& circle_collider0, components::CircleColliderComponent& circle_collider1)
			{
				auto pos0 = circle_collider0.GetCollisionCenterPosition();
				auto pos1 = circle_collider1.GetCollisionCenterPosition();
				double circle_radius0 = circle_collider0.radius()*circle_collider0.GetCollisionScale();
				double circle_radius1 = circle_collider1.radius()*circle_collider1.GetCollisionScale();
				if (Vector2D<double>::Length_Square(pos1 - pos0) <= std::pow(circle_radius0 + circle_radius1, 2)) {
					return true;
				}
				else { return false; }
			}

			bool CollisionDetectCircleAndStraightLine(components::CircleColliderComponent& circle_collider, components::StraightLineColliderComponent& straightline_collider)
			{
				double circle_radius = circle_collider.radius()*circle_collider.GetCollisionScale();
				auto circle_pos = circle_collider.GetCollisionCenterPosition();
				auto line_start_pos = straightline_collider.GetCollisionCenterPosition();
				auto line_vector = Vector2D<double>(std::cos(straightline_collider.GetCollisionRotationRad()), std::sin(straightline_collider.GetCollisionRotationRad()))*straightline_collider.GetCollisionScale()*straightline_collider.length();
				auto line_start_to_circle_center = circle_pos - line_start_pos;
				//円と線分の距離を求める
				double distance = std::abs(Vector2D<double>::Cross(line_vector, line_start_to_circle_center) / line_vector.Length());
				if (distance > circle_radius) { return false; }
				else {
					//円が線分の両端外側にあるか
					double line_start_to_circle_center_dot_line_vector = Vector2D<double>::Dot(line_start_to_circle_center, line_vector);
					Vector2D<double> line_end_to_circle_center = line_start_to_circle_center - line_vector;
					if (line_start_to_circle_center_dot_line_vector*Vector2D<double>::Dot(line_end_to_circle_center, line_vector) <= 0) { //線分内にある
						return true;
					}
					else { //線分外にある
						if (line_start_to_circle_center_dot_line_vector > 0) { //線分の終点側にある
							if (line_end_to_circle_center.Length() <= circle_radius) { return true; }
							else { return false; }
						}
						else { //線分の始点側にある
							if (line_start_to_circle_center.Length() <= circle_radius) { return true; }
							else { return false; }
						}
					}
				}
			}

		}
	}
}