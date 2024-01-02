#include "standard_scene_manager_utility.hpp"
#include "scene.hpp"
#include "standard_collision_world.hpp"
#include "standard_draw_system.hpp"
#include "standard_task_manager.hpp"
#include "standard_game_object_manager.hpp"
#include "standard_transform_system.hpp"

namespace plnt::private_ {
	void set_standard_scene_modules(scene &scene) {
		scene.set_collision_world(std::make_shared<standard_collision_world>());
		scene.set_draw_system(std::make_shared<standard_draw_system>());
		scene.set_task_manager(std::make_shared<standard_task_manager>());
		scene.set_game_object_manager(std::make_shared<standard_game_object_manager>());
		scene.set_transform_manager(std::make_shared<standard_transform_system>());
	}
}
