#pragma once

#include "planeta/core/Object.hpp"
#include "planeta/core/WeakPointer.hpp"

namespace plnt {
	class CCollider2D;
	class EACollisionWithCollider2D : public Object {
	public:
		EACollisionWithCollider2D(CCollider2D& cc) :collider(cc) {}
		CCollider2D& collider;
	};
}