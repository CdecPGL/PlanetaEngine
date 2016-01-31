#pragma once

#include "ColliderComponent.h"

namespace planeta_engine {
	namespace components {
		class CircleColliderComponent final: public ColliderComponent {
		public:
			bool DetectCollision(core::IColliderWithCollider& collider)override { return collider.CollideWith(*this); }
			const double radius()const { return _radius; }
			CircleColliderComponent& radius(double r) { _radius = r; return *this; }
		private:
			virtual bool CollideWith(CircleColliderComponent& circle_collider) override;
			virtual bool CollideWith(StraightLineColliderComponent& straight_line_collider) override;


			double _radius = 1.0;
		};
	}
}
