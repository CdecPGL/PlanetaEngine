#pragma once

#include "i_scene.hpp"
#include "WeakPointer.hpp"

namespace plnt::private_ {
	class collision_world;
	class draw_system;
	class game_object_manager;
	class task_manager;
	class transform_system;

	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_scene_internal : public i_scene {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		~i_scene_internal() override = 0 { }

		virtual weak_pointer<private_::collision_world> collision_world_internal_pointer() = 0;
		virtual weak_pointer<private_::draw_system> draw_system_internal_pointer() = 0;
		virtual weak_pointer<private_::game_object_manager> game_object_manager_internal_pointer() = 0;
		virtual weak_pointer<private_::task_manager> task_manager_internal_pointer() = 0;
		virtual weak_pointer<private_::transform_system> transform_system_internal_pointer() = 0;
	};
}
