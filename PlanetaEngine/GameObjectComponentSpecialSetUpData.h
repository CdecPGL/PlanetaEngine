#pragma once

#include <memory>

#include "WeakPointer.h"

namespace planeta_engine {
	class ScreenDrawer2D;
	namespace system_processes{
		class CollisionDetectProcess;
	}
	namespace core {
		class GameObjectDrawComponentProcessRegistrator;
		struct GameObjectComponentSpecialSetUpData {
			std::shared_ptr<ScreenDrawer2D> screen_drawer_2d;
			utility::WeakPointer<system_processes::CollisionDetectProcess> collision_detect_process;
			std::shared_ptr<GameObjectDrawComponentProcessRegistrator> draw_component_process_registrator;
		};
	}
}
