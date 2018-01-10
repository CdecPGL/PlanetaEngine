#pragma once

#include "planeta/Object.hpp"
#include "WeakPointer.hpp"

namespace plnt {
	class CCollider2D;
	class EACollisionWithCollider2D : public Object {
	public:
		EACollisionWithCollider2D(CCollider2D& cc) :collider(cc) {}
		CCollider2D& collider;
	};
}