#pragma once

#include "NonOwingPointer.h"

namespace planeta {
	class SceneData;
	class IGameObject;
	namespace private_ {
		struct GameObjectComponentSetUpData {
			NonOwingPointer<IGameObject> holder_game_object;
			WeakPointer<SceneData> scene_data;
		};
	}
}
