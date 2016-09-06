#pragma once

#include <memory>
#include "WeakPointer.h"

namespace planeta {
	class GameObjectManagerPublicInterface;
	class TaskManagerPublicInterface;
	class ISceneManagerAccessor;
	namespace private_ {
		class CollisionWorld;
		class GameObjectDrawSystem;
		class TransformSystem;
	}
	namespace private_ {
		struct SceneData {
			GameObjectManagerPublicInterface& game_object_manager_public_interface;
			TaskManagerPublicInterface& task_manager_public_interface;
			private_::CollisionWorld& collision_world;
			private_::GameObjectDrawSystem& gameobject_draw_system;
			private_::TransformSystem& transform_system;
			ISceneManagerAccessor& scene_manager;
		};
	}
}