#pragma once

#include "planeta/core/Object.hpp"
#include "CollisionState.hpp"


namespace plnt {
	class EACollisionWithGround2D : public Object {
	public:
		CollisionState collision_state;
	private:
	};
}
