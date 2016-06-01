#pragma once

#include "Object.h"
#include "WeakPointer.h"

namespace planeta {
	class IGameObject;
	class EACollisionWithCollider2D : public core::Object {
	public:
		EACollisionWithCollider2D(IGameObject& go) :game_object(go) {}
		IGameObject& game_object;
	};
}