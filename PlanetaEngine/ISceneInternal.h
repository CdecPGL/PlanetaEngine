#pragma once

#include "IScene.h"
#include "WeakPointer.h"

namespace planeta {
	namespace private_ {
		class CollisionWorld;
		class DrawSystem;
		class GameObjectManager;
		class TaskManager;
		class TransformSystem;
		class ISceneInternal : public IScene {
		public:
			virtual ~ISceneInternal() = 0 {};
			virtual WeakPointer<CollisionWorld> collision_world_internal_pointer() = 0;
			virtual WeakPointer<DrawSystem> draw_system_internal_pointer() = 0;
			virtual WeakPointer<GameObjectManager> game_object_manager_internal_pointer() = 0;
			virtual WeakPointer<TaskManager> task_manager_internal_pointer() = 0;
			virtual WeakPointer<TransformSystem> transform_system_internal_pointer() = 0;
		};
	}
}
