﻿#pragma once

#include "object.hpp"
#include "collision_state.hpp"


namespace plnt {
	class e_collision_with_ground_2d final : public object {
	public:
		collision_state collision_state{};
	};
}
