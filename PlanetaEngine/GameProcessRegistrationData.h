#pragma once

#include <functional>
#include "WeakPointer.h"
#include "GameProcessPositionInList.h"

namespace planeta_engine {
	namespace core {
		class SceneAccessorForGameProcess;
		struct GameProcessRegistrationData {
			utility::WeakPointer<SceneAccessorForGameProcess> scene_accessor;
			std::function<void()> disposer;
			GameProcessPositionInList position_in_list;
		};
	}
}
