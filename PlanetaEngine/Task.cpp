#include "Task.h"
#include "TaskManagerConnection.h"
#include "SceneDataForTask.h"

namespace planeta_engine {
	Task::~Task() = default;

	void Task::Dispose() {
		disposed(); //イベント実行
		OnDisposed(); //自分のイベント関数実行
		manager_connection_->Dispose(); //破棄
	}

	bool Task::SystemSetUpAndInitialize(std::unique_ptr<core::TaskManagerConnection>&& manager_connection, const core::SceneDataForTask& special_setup_data) {
		manager_connection_ = std::move(manager_connection);
		return OnCreated();
	}

	SceneAccessorForTask& Task::scene() {
		return *manager_connection_->ReferenceSceneAccessor();
	}

	bool Task::Pause() {
		return manager_connection_->Pause();
	}

	bool Task::Resume() {
		return manager_connection_->Resume();
	}

}
