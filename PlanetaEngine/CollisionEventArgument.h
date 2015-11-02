#pragma once

#include "Object.h"
#include "WeakPointer.h"

namespace planeta_engine {
	namespace game {
		class IGameObjectAccessor;
	}
	namespace event_arguments {
		class CollisionEventArgument : public core::Object{
		public:
			CollisionEventArgument(game::IGameObjectAccessor& go) :game_object(go) {}
			game::IGameObjectAccessor& game_object;
		};
	}
}