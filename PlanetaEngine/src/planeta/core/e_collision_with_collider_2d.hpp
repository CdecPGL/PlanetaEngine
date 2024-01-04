#pragma once

#include "object.hpp"

namespace plnt {
	class c_collider_2d;

	class e_collision_with_collider_2d final : public object {
	public:
		explicit e_collision_with_collider_2d(c_collider_2d &cc) : collider(cc) { }

		c_collider_2d &collider;
	};
}
