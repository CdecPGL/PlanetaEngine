#include "StandardSceneManagerUtility.hpp"
#include "Scene.hpp"

#include "StandardCollisionWorld.hpp"
#include "StandardDrawSystem.hpp"
#include "StandardTaskManager.hpp"
#include "StandardGameObjectManager.hpp"
#include "StandardTransformSystem.hpp"

namespace plnt {
	namespace private_ {
		void SetStandardSceneModules(scene &scene) {
			scene.set_collision_world(std::make_shared<StandardCollisionWorld>());
			scene.set_draw_system(std::make_shared<StandardDrawSystem>());
			scene.set_task_manager(std::make_shared<StandardTaskManager>());
			scene.set_game_object_manager(std::make_shared<StandardGameObjectManager>());
			scene.set_transform_manager(std::make_shared<StandardTransformSystem>());
		}
	}
}
