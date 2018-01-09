#pragma once

#include "Object.hpp"
#include "CollisionState.hpp"


namespace planeta {
	class EACollisionWithGround2D : public Object {
	public:
		CollisionState collision_state;
	private:
	};
}
