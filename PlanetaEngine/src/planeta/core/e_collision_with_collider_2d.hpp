#pragma once

#include "object.hpp"

namespace plnt {
	class CCollider2D;

	class e_collision_with_collider_2d final : public object {
	public:
		explicit e_collision_with_collider_2d(CCollider2D &cc) : collider(cc) { }

		CCollider2D &collider;
	};
}
