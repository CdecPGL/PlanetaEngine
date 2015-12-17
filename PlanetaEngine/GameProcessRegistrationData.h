#pragma once

#include <functional>
#include "WeakPointer.h"
#include "GameProcessPosition.h"

namespace planeta_engine {
	namespace core {
		class SceneAccessorForGameProcess;
		struct GameProcessRegistrationData {
			utility::WeakPointer<SceneAccessorForGameProcess> scene_accessor;
			std::function<void()> disposer;
			GameProcessPosition position_in_list;
		};
	}
}
