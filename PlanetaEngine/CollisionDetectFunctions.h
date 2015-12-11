#pragma once

#include <memory>

namespace planeta_engine {
	namespace components {
		class CircleColliderComponent;
		class StraightLineColliderComponent;
	}
	namespace core {
		namespace collision_detect_functions {
			bool CollisionDetectCircleAndCirecle(components::CircleColliderComponent& circle_collider0, components::CircleColliderComponent& circle_collider1);
			bool CollisionDetectCircleAndStraightLine(components::CircleColliderComponent& circle_collider, components::StraightLineColliderComponent& straightline_collider);
		}
	}
}
