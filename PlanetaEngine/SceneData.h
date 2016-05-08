#pragma once

#include <memory>
#include "WeakPointer.h"

namespace planeta_engine {
	class GameObjectManagerPublicInterface;
	class TaskManagerPublicInterface;
	namespace core {
		class CollisionWorld;
		class GameObjectDrawSystem;
		class TransformSystem;
	}
	namespace core {
		struct SceneData {
			GameObjectManagerPublicInterface& game_object_manager_public_interface;
			TaskManagerPublicInterface& task_manager_public_interface;
			core::CollisionWorld& collision_world;
			core::GameObjectDrawSystem& gameobject_draw_system;
			core::TransformSystem& transform_system;
		};
	}
}