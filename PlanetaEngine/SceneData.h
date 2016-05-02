#pragma once

#include <memory>
#include "WeakPointer.h"

namespace planeta_engine {
	class ScreenDrawer2D;
	class ScreenDrawerGUI;
	class GameObjectManagerPublicInterface;
	class TaskManagerPublicInterface;
	namespace core {
		class CollisionWorld;
		class GameObjectDrawProcessCore;
	}
	namespace core {
		struct SceneData {
			GameObjectManagerPublicInterface& game_object_manager_public_interface;
			TaskManagerPublicInterface& task_manager_public_interface;
			utility::WeakPointer<core::CollisionWorld> collision_world;
			utility::WeakPointer<core::GameObjectDrawProcessCore> draw_component_process_registrator;
			std::shared_ptr<ScreenDrawer2D> screen_drawer_2d;
		};
	}
}