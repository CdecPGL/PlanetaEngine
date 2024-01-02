#include "StandardSceneManagerUtility.hpp"
#include "scene.hpp"

#include "standard_collision_world.hpp"
#include "StandardDrawSystem.hpp"
#include "StandardTaskManager.hpp"
#include "StandardGameObjectManager.hpp"
#include "StandardTransformSystem.hpp"

namespace plnt {
	namespace private_ {
		void SetStandardSceneModules(scene &scene) {
			scene.set_collision_world(std::make_shared<standard_collision_world>());
			scene.set_draw_system(std::make_shared<standard_draw_system>());
			scene.set_task_manager(std::make_shared<StandardTaskManager>());
			scene.set_game_object_manager(std::make_shared<standard_game_object_manager>());
			scene.set_transform_manager(std::make_shared<StandardTransformSystem>());
		}
	}
}
