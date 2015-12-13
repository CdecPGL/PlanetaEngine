#pragma once

#include <memory>
#include "WeakPointer.h"

namespace planeta_engine {
	class ScreenDrawer2D;
	class ScreenDrawerUI;
	class Camera;
	namespace system_processes {
		class CollisionDetectProcess;
		class GameObjectDrawProcess;
	}
	namespace core {
		struct SceneData {
			utility::WeakPointer<system_processes::CollisionDetectProcess> collision_detect_process;
			utility::WeakPointer<system_processes::GameObjectDrawProcess> draw_component_process_registrator;
			std::shared_ptr<ScreenDrawer2D> screen_drawer_2d;
			std::shared_ptr<ScreenDrawerUI> screen_drawer_ui;
			std::shared_ptr<Camera> camera;
		};
	}
}