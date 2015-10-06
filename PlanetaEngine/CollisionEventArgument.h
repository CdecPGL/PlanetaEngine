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
			CollisionEventArgument(const utility::WeakPointer<game::IGameObjectAccessor>& go) :game_object(go) {}
			const utility::WeakPointer<game::IGameObjectAccessor> game_object;
		};
	}
}