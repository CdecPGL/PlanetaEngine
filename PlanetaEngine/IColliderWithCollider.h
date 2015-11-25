#pragma once

namespace planeta_engine {
	namespace components {
		class CircleColliderComponent;
		class StraightLineColliderComponent;
		class PolygonColliderComponent;
	}
	namespace core {
		/*コライダーと衝突するコライダーのインターフェイス*/
		class IColliderWithCollider {
		public:
			virtual bool CollideWith(components::CircleColliderComponent& circle_collider) { return false; }
			virtual bool CollideWith(components::StraightLineColliderComponent& straight_line_collider) { return false; }
			virtual bool CollideWith(components::PolygonColliderComponent& polygon_collider) { return false; }
		};
	}
}