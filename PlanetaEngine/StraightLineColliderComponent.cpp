#include "StraightLineColliderComponent.h"
#include "CollisionDetectFunctions.h"

namespace planeta_engine {
	namespace components {
		bool StraightLineColliderComponent::CollideWith(CircleColliderComponent& circle_collider) {
			return game::collision_detect_functions::CollisionDetectCircleAndStraightLine(circle_collider, *this);
		}
	}
}