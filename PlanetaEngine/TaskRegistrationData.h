#pragma once

#include <functional>
#include "WeakPointer.h"
#include "TaskPosition.h"

namespace planeta_engine {
	class SceneAccessorForTask;
	namespace core {
		struct TaskRegistrationData {
			utility::WeakPointer<SceneAccessorForTask> scene_accessor;
			std::function<void()> disposer;
			TaskPosition position_in_list;
		};
	}
}
