#include "SceneSetUpProxy.h"
#include "Scene.h"
#include "SceneData.h"
#include "GameObjectManagerPublicInterface.h"
#include "IGameObject.h"

namespace planeta_engine {

	SceneSetUpProxy::SceneSetUpProxy(core::Scene& scene):scene_(scene) {}

	TaskManagerPublicInterface& SceneSetUpProxy::RefTaskManagerInterface_() {
		return scene_.RefSceneData().task_manager_public_interface;
	}

	const TaskManagerPublicInterface& SceneSetUpProxy::RefTaskManagerInterface_()const {
		return scene_.RefSceneData().task_manager_public_interface;
	}

	utility::WeakPointer<Task> SceneSetUpProxy::GetTaskByName(const std::string& name) const {
		return RefTaskManagerInterface_().GetTask(name);
	}

	utility::WeakPointer<IGameObject> SceneSetUpProxy::CreateGameObject(const std::string& id) {
		return scene_.RefSceneData().game_object_manager_public_interface.CreateGameObject(id);
	}

	utility::WeakPointer<IGameObject> SceneSetUpProxy::CreateAndActivateGameObject(const std::string& id) {
		auto go = scene_.RefSceneData().game_object_manager_public_interface.CreateGameObject(id);
		go->Activate();
		return go;
	}

}