#include "Task.h"
#include "TaskManagerConnection.h"
#include "SceneData.h"
#include "GameObjectManagerPublicInterface.h"
#include "IGameObject.h"


namespace planeta {
	Task::Task() {}
	Task::~Task() = default;

	void Task::Dispose() {
		disposed(); //イベント実行
		OnDisposed(); //自分のイベント関数実行
		manager_connection_->Dispose(); //破棄
	}

	bool Task::SystemSetUpAndInitialize(std::unique_ptr<core::TaskManagerConnection>&& manager_connection, const util::WeakPointer<core::SceneData>& scene_data) {
		scene_data_ = scene_data;
		manager_connection_ = std::move(manager_connection);
		return OnCreated();
	}

	bool Task::Pause() {
		return manager_connection_->Pause();
	}

	bool Task::Resume() {
		return manager_connection_->Resume();
	}

	TaskManagerPublicInterface& Task::RefTaskManagerInterface_() {
		return scene_data_->task_manager_public_interface;
	}

	const TaskManagerPublicInterface& Task::RefTaskManagerInterface_()const {
		return scene_data_->task_manager_public_interface;
	}

	util::WeakPointer<Task> Task::GetTaskByName(const std::string& name) const {
		return RefTaskManagerInterface_().GetTask(name);
	}

	util::WeakPointer<IGameObject> Task::CreateGameObject(const std::string& id, const std::string& resource_id) {
		return scene_data_->game_object_manager_public_interface.CreateGameObject(id, resource_id);
	}

	util::WeakPointer<IGameObject> Task::CreateAndActivateGameObject(const std::string& id, const std::string& resource_id) {
		auto go = scene_data_->game_object_manager_public_interface.CreateGameObject(id, resource_id);
		if (go == nullptr) { return nullptr; }
		go->Activate();
		return go;
	}

	util::WeakPointer<IGameObject> Task::CreateDefaultGameObject(const std::string& id) {
		return scene_data_->game_object_manager_public_interface.CreateDefaultGameObject(id);
	}

	util::WeakPointer<IGameObject> Task::CreateAndActivateDefaultGameObject(const std::string& id) {
		auto go = scene_data_->game_object_manager_public_interface.CreateDefaultGameObject(id);
		if (go == nullptr) { return nullptr; }
		go->Activate();
		return go;
	}

	ISceneManagerAccessor& Task::scene_manager() {
		return scene_data_->scene_manager;
	}

}
