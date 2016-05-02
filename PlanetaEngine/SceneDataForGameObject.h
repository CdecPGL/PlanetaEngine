#pragma once

#include <memory>

#include "WeakPointer.h"

namespace planeta_engine {
	namespace system_processes{
		class CollisionWorld;
		class GameObjectDrawSystem;
	}
	namespace core {
		struct SceneDataForGameObject {
			utility::WeakPointer<system_processes::CollisionWorld> collision_detect_process;
			utility::WeakPointer<system_processes::GameObjectDrawSystem> draw_component_process_registrator;
		};
	}
}
