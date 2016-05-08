#pragma once

#include "Object.h"
#include "WeakPointer.h"

namespace planeta_engine {
	class IGameObject;
	namespace event_arguments {
		class CollisionEventArgument : public core::Object {
		public:
			CollisionEventArgument(IGameObject& go) :game_object(go) {}
			IGameObject& game_object;
		};
	}
}