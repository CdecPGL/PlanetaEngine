#pragma once

#include "Object.h"
#include "WeakPointer.h"

namespace planeta {
	class CCollider2D;
	class EACollisionWithCollider2D : public Object {
	public:
		EACollisionWithCollider2D(CCollider2D& cc) :collider(cc) {}
		CCollider2D& collider;
	};
}