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

	bool Task::SystemSetUpAndInitialize(std::unique_ptr<private_::TaskManagerConnection>&& manager_connection, const WeakPointer<private_::SceneData>& scene_data) {
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

	WeakPointer<Task> Task::GetTaskByName(const std::string& name) const {
		return RefTaskManagerInterface_().GetTask(name);
	}

	planeta::GameObjectManagerPublicInterface& Task::game_object_manager() {
		return scene_data_->game_object_manager_public_interface;
	}

	ISceneManagerAccessor& Task::scene_manager() {
		return scene_data_->scene_manager;
	}

}
