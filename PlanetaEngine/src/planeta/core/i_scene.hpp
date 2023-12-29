#pragma once

namespace plnt {
	class i_collision_world;
	class i_draw_system;
	class i_game_object_manager;
	class i_task_manager;
	class i_transform_system;

	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class i_scene {
	public:
		// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
		virtual ~i_scene() = 0 { }

		virtual i_collision_world &collision_world() = 0;
		virtual i_draw_system &draw_system() = 0;
		virtual i_game_object_manager &game_object_manager() = 0;
		virtual i_task_manager &task_manager() = 0;
		virtual i_transform_system &transform_system() = 0;
	};
}
