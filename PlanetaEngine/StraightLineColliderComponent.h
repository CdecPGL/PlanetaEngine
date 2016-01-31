#pragma once

#include "ColliderComponent.h"

namespace planeta_engine {
	namespace components {
		class StraightLineColliderComponent final: public ColliderComponent {
		public:
			bool DetectCollision(core::IColliderWithCollider& collider)override { return collider.CollideWith(*this); }
			const double length()const { return length_; }
			StraightLineColliderComponent& length(double l) { length_ = l; return *this; }
		private:
			virtual bool CollideWith(CircleColliderComponent& circle_collider) override;

			double length_ = 1.0;
		};
	}
}
