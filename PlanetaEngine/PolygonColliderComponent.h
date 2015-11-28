#pragma once
#include "ColliderComponent.h"

namespace planeta_engine {
	namespace components {
		/*‘½ŠpŒ`Õ“ËƒNƒ‰ƒX(–¢À‘•)*/
		class PolygonColliderComponent : public ColliderComponent {
			bool DetectCollision(core::IColliderWithCollider& collider)override { return collider.CollideWith(*this); }
		};
	}
}
