#include "StandardSceneManagerUtility.hpp"
#include "Scene.hpp"

#include "StandardCollisionWorld.hpp"
#include "StandardDrawSystem.hpp"
#include "StandardTaskManager.hpp"
#include "StandardGameObjectManager.hpp"
#include "StandardTransformSystem.hpp"

namespace plnt {
	namespace private_ {
		void SetStandardSceneModules(Scene &scene) {
			scene.SetCollisionWorld(std::make_shared<StandardCollisionWorld>());
			scene.SetDrawSystem(std::make_shared<StandardDrawSystem>());
			scene.SetTaskManager(std::make_shared<StandardTaskManager>());
			scene.SetGameObjectManager(std::make_shared<StandardGameObjectManager>());
			scene.SetTransformManager(std::make_shared<StandardTransformSystem>());
		}
	}
}
