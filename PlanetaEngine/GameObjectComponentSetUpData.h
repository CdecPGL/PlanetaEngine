#pragma once

#include "NonOwingPointer.h"

namespace planeta {
	class SceneData;
	class IGameObjectForComponent;
	namespace private_ {
		struct GameObjectComponentSetUpData {
			NonOwingPointer<IGameObjectForComponent> holder_game_object;
			WeakPointer<SceneData> scene_data;
		};
	}
}
