#pragma once

#include <memory>

#include "WeakPointer.h"

namespace planeta_engine {
	namespace system_processes{
		class TCollisionDetect;
		class GameObjectDrawProcessCore;
	}
	namespace core {
		struct SceneDataForGameObject {
			utility::WeakPointer<system_processes::TCollisionDetect> collision_detect_process;
			utility::WeakPointer<system_processes::GameObjectDrawProcessCore> draw_component_process_registrator;
		};
	}
}
