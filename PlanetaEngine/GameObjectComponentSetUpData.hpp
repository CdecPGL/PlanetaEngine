#pragma once

#include "NonOwingPointer.h"

namespace planeta {
	class ISceneInternal;
	class IGameObject;
	namespace private_ {
		struct GameObjectComponentSetUpData {
			NonOwingPointer<IGameObject> holder_game_object;
			WeakPointer<ISceneInternal> scene_internal_interface;
		};
	}
}
