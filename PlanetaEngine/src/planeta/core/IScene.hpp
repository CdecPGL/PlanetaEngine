#pragma once


namespace plnt {
	class ICollisionWorld;
	class IDrawSystem;
	class IGameObjectManager;
	class ITaskManager;
	class ITransformSystem;

	class IScene {
	public:
		virtual ~IScene() = 0 { };
		virtual ICollisionWorld &collision_world() = 0;
		virtual IDrawSystem &draw_system() = 0;
		virtual IGameObjectManager &game_object_manager() = 0;
		virtual ITaskManager &task_manager() = 0;
		virtual ITransformSystem &transform_system() = 0;
	};
}
