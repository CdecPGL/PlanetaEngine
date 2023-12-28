#pragma once

#include "Object.hpp"
#include "collision_state.hpp"


namespace plnt {
	class e_collision_with_ground_2d final : public Object {
	public:
		collision_state collision_state{};
	};
}
