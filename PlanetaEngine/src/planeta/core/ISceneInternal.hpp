#pragma once

#include "IScene.hpp"
#include "WeakPointer.hpp"

namespace plnt {
	namespace private_ {
		class collision_world;
		class draw_system;
		class game_object_manager;
		class TaskManager;
		class TransformSystem;

		class ISceneInternal : public IScene {
		public:
			virtual ~ISceneInternal() = 0 { };
			virtual WeakPointer<private_::collision_world> collision_world_internal_pointer() = 0;
			virtual WeakPointer<private_::draw_system> draw_system_internal_pointer() = 0;
			virtual WeakPointer<private_::game_object_manager> game_object_manager_internal_pointer() = 0;
			virtual WeakPointer<TaskManager> task_manager_internal_pointer() = 0;
			virtual WeakPointer<TransformSystem> transform_system_internal_pointer() = 0;
		};
	}
}
