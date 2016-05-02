#pragma once

#include "WeakPointer.h"

namespace planeta_engine {
	class SceneAccessorForGameObject;
	class IGameObject;
	namespace core {
		struct GameObjectComponentRegistrationData {
			int id;
			utility::WeakPointer<IGameObject> holder_game_object;
		};
	}
}
