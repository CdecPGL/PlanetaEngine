#pragma once

#include "WeakPointer.h"

namespace planeta_engine {
	class SceneAccessorForGameObject;
	class GameObject;
	namespace core {
		struct GameObjectComponentRegistrationData {
			int id;
			utility::WeakPointer<SceneAccessorForGameObject> scene_accessor;
			utility::WeakPointer<GameObject> holder_game_object;
		};
	}
}
