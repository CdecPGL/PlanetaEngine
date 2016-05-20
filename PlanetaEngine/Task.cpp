#include "Task.h"
#include "TaskManagerConnection.h"
#include "SceneData.h"
#include "GameObjectManagerPublicInterface.h"
#include "IGameObject.h"


namespace planeta_engine {
	Task::Task() {}
	Task::~Task() = default;

	void Task::Dispose() {
		disposed(); //�C�x���g���s
		OnDisposed(); //�����̃C�x���g�֐����s
		manager_connection_->Dispose(); //�j��
	}

	bool Task::SystemSetUpAndInitialize(std::unique_ptr<core::TaskManagerConnection>&& manager_connection, const utility::WeakPointer<core::SceneData>& scene_data,core::IGameAccessor& game) {
		game_.reset(&game);
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

	utility::WeakPointer<Task> Task::GetTaskByName(const std::string& name) const {
		return RefTaskManagerInterface_().GetTask(name);
	}

	utility::WeakPointer<IGameObject> Task::CreateGameObject(const std::string& id) {
		return scene_data_->game_object_manager_public_interface.CreateGameObject(id);
	}

	utility::WeakPointer<IGameObject> Task::CreateAndActivateGameObject(const std::string& id) {
		auto go = scene_data_->game_object_manager_public_interface.CreateGameObject(id);
		go->Activate();
		return go;
	}

	core::IGameAccessor& Task::game_accessor() {
		return *game_;
	}

}
