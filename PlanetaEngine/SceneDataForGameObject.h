#pragma once

#include <memory>

#include "WeakPointer.h"

namespace planeta_engine {
	namespace system_processes{
		class CollisionDetectTask;
		class GameObjectDrawProcessCore;
	}
	namespace core {
		class GameObjectDrawComponentProcessRegistrator;
		struct SceneDataForGameObject {
			utility::WeakPointer<system_processes::CollisionDetectTask> collision_detect_process;
			utility::WeakPointer<system_processes::GameObjectDrawProcessCore> draw_component_process_registrator;
//			std::shared_ptr<GameObjectDrawComponentProcessRegistrator> draw_component_process_registrator;
		};
	}
}
