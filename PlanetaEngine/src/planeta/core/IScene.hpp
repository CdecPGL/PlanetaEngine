#pragma once


namespace plnt {
	class i_collision_world;
	class i_draw_system;
	class i_game_object_manager;
	class ITaskManager;
	class ITransformSystem;

	class IScene {
	public:
		virtual ~IScene() = 0 { };
		virtual i_collision_world &collision_world() = 0;
		virtual i_draw_system &draw_system() = 0;
		virtual i_game_object_manager &game_object_manager() = 0;
		virtual ITaskManager &task_manager() = 0;
		virtual ITransformSystem &transform_system() = 0;
	};
}
