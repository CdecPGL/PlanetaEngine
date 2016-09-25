#include "SceneSetUpProxy.h"
#include "Scene.h"
#include "SceneData.h"
#include "GameObjectManagerPublicInterface.h"
#include "IGameObject.h"

namespace planeta {

	SceneSetUpProxy::SceneSetUpProxy(private_::Scene& scene):scene_(scene) {}

	planeta::GameObjectManagerPublicInterface& SceneSetUpProxy::game_object_manager() {
		return scene_.RefSceneData().game_object_manager_public_interface;
	}

	TaskManagerPublicInterface& SceneSetUpProxy::RefTaskManagerInterface_() {
		return scene_.RefSceneData().task_manager_public_interface;
	}

	const TaskManagerPublicInterface& SceneSetUpProxy::RefTaskManagerInterface_()const {
		return scene_.RefSceneData().task_manager_public_interface;
	}

	WeakPointer<Task> SceneSetUpProxy::GetTaskByName(const std::string& name) const {
		return RefTaskManagerInterface_().GetTask(name);
	}
}