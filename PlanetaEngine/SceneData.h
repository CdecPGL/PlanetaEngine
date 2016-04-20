#pragma once

#include <memory>
#include "WeakPointer.h"

namespace planeta_engine {
	class ScreenDrawer2D;
	class ScreenDrawerGUI;
	namespace system_processes {
		class CollisionDetectTask;
		class GameObjectDrawProcessCore;
	}
	namespace core {
		struct SceneData {
			utility::WeakPointer<system_processes::CollisionDetectTask> collision_detect_process;
			utility::WeakPointer<system_processes::GameObjectDrawProcessCore> draw_component_process_registrator;
			std::shared_ptr<ScreenDrawer2D> screen_drawer_2d;
			std::shared_ptr<ScreenDrawerGUI> screen_drawer_ui;
		};
	}
}