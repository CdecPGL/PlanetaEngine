#pragma once

#include "i_scene.hpp"
#include "WeakPointer.hpp"

namespace plnt::private_ {
	class collision_world;
	class draw_system;
	class game_object_manager;
	class TaskManager;
	class TransformSystem;

	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_scene_internal : public i_scene {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		~i_scene_internal() override = 0 { }

		virtual WeakPointer<private_::collision_world> collision_world_internal_pointer() = 0;
		virtual WeakPointer<private_::draw_system> draw_system_internal_pointer() = 0;
		virtual WeakPointer<private_::game_object_manager> game_object_manager_internal_pointer() = 0;
		virtual WeakPointer<TaskManager> task_manager_internal_pointer() = 0;
		virtual WeakPointer<TransformSystem> transform_system_internal_pointer() = 0;
	};
}
