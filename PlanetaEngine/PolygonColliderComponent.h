#pragma once
#include "ColliderComponent.h"

namespace planeta_engine {
	namespace components {
		/*���p�`�Փ˃N���X(������)*/
		class PolygonColliderComponent : public ColliderComponent {
			bool DetectCollision(core::IColliderWithCollider& collider)override { return collider.CollideWith(*this); }
		};
	}
}
