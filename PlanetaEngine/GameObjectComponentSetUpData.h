#pragma once

#include "NonOwingPointer.h"

namespace planeta {
	class SceneData;
	class IGameObjectForComponent;
	namespace private_ {
		struct GameObjectComponentSetUpData {
			util::NonOwingPointer<IGameObjectForComponent> holder_game_object;
			util::WeakPointer<SceneData> scene_data;
		};
	}
}
