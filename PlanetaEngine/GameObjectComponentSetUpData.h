#pragma once

#include "NonOwingPointer.h"

namespace planeta_engine {
	class SceneData;
	class IGameObjectForComponent;
	namespace core {
		struct GameObjectComponentSetUpData {
			util::NonOwingPointer<IGameObjectForComponent> holder_game_object;
			util::WeakPointer<SceneData> scene_data;
		};
	}
}
