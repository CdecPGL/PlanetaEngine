#pragma once

#include "WeakPointer.h"

namespace planeta_engine {
	namespace game {
		class GameObject;
	}
	namespace core {
		class SceneAccessorForGameObject;
		struct GameObjectComponentRegistrationData {
			int id;
			utility::WeakPointer<SceneAccessorForGameObject> scene_accessor;
			utility::WeakPointer<game::GameObject> holder_game_object;
		};
	}
}
