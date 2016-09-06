#include "SceneSetUpProxy.h"
#include "Scene.h"
#include "SceneData.h"
#include "GameObjectManagerPublicInterface.h"
#include "IGameObject.h"

namespace planeta {

	SceneSetUpProxy::SceneSetUpProxy(private_::Scene& scene):scene_(scene) {}

	TaskManagerPublicInterface& SceneSetUpProxy::RefTaskManagerInterface_() {
		return scene_.RefSceneData().task_manager_public_interface;
	}

	const TaskManagerPublicInterface& SceneSetUpProxy::RefTaskManagerInterface_()const {
		return scene_.RefSceneData().task_manager_public_interface;
	}

	util::WeakPointer<Task> SceneSetUpProxy::GetTaskByName(const std::string& name) const {
		return RefTaskManagerInterface_().GetTask(name);
	}

	util::WeakPointer<IGameObject> SceneSetUpProxy::CreateGameObject(const std::string& id) {
		return scene_.RefSceneData().game_object_manager_public_interface.CreateDefaultGameObject(id);
	}

	util::WeakPointer<IGameObject> SceneSetUpProxy::CreateAndActivateGameObject(const std::string& id) {
		auto go = scene_.RefSceneData().game_object_manager_public_interface.CreateDefaultGameObject(id);
		if (go == nullptr) { return nullptr; }
		go->Activate();
		return go;
	}

}