#pragma once

#include "WeakPointer.h"

namespace planeta_engine {
	class SceneAccessorForGameObject;
	namespace game {
		class GameObject;
	}
	namespace core {
		struct GameObjectComponentRegistrationData {
			int id;
			utility::WeakPointer<SceneAccessorForGameObject> scene_accessor;
			utility::WeakPointer<game::GameObject> holder_game_object;
		};
	}
}
