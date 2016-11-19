#include "StandardSceneManagerUtility.h"
#include "Scene.h"

#include "StandardCollisionWorld.h"
#include "StandardDrawSystem.h"
#include "StandardTaskManager.h"
#include "StandardGameObjectManager.h"
#include "StandardTransformSystem.h"

namespace planeta {
	namespace private_ {
		void SetStandardSceneModules(Scene& scene) {
			scene.SetCollisionWorld(std::make_shared<StandardCollisionWorld>());
			scene.SetDrawSystem(std::make_shared<StandardDrawSystem>());
			scene.SetTaskManager(std::make_shared<StandardTaskManager>());
			scene.SetGameObjectManager(std::make_shared<StandardGameObjectManager>());
			scene.SetTransformManager(std::make_shared<StandardTransformSystem>());
		}
	}
}