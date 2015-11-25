#include "CircleColliderComponent.h"
#include "CollisionDetectFunctions.h"

namespace planeta_engine {
	namespace components {
		bool CircleColliderComponent::CollideWith(CircleColliderComponent& circle_collider) {
			return game::collision_detect_functions::CollisionDetectCircleAndCirecle(circle_collider, *this);
		}

		bool CircleColliderComponent::CollideWith(StraightLineColliderComponent& straight_line_collider) {
			return game::collision_detect_functions::CollisionDetectCircleAndStraightLine(*this, straight_line_collider);
		}

	}
}