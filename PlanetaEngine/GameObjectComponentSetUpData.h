#pragma once

#include "NonOwingPointer.h"

namespace planeta_engine {
	class SceneData;
	class IGameObjectForComponent;
	namespace core {
		struct GameObjectComponentSetUpData {
			utility::NonOwingPointer<IGameObjectForComponent> holder_game_object;
			utility::WeakPointer<SceneData> scene_data;
		};
	}
}
