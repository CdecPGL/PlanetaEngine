#pragma once

#include "Object.h"
#include "WeakPointer.h"

namespace planeta_engine {
	class IGameObjectAccessor;
	namespace event_arguments {
		class CollisionEventArgument : public core::Object {
		public:
			CollisionEventArgument(IGameObjectAccessor& go) :game_object(go) {}
			IGameObjectAccessor& game_object;
		};
	}
}