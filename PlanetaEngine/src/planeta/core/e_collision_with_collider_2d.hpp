#pragma once

#include "Object.hpp"

namespace plnt {
	class CCollider2D;

	class e_collision_with_collider_2d final : public Object {
	public:
		explicit e_collision_with_collider_2d(CCollider2D &cc) : collider(cc) { }

		CCollider2D &collider;
	};
}
