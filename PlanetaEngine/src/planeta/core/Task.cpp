#include "Task.hpp"
#include "TaskManagerConnection.hpp"
#include "i_game_object.hpp"


namespace plnt {
	Task::Task() { }

	Task::~Task() = default;

	void Task::Dispose() {
		disposed(); //イベント実行
		OnDisposed(); //自分のイベント関数実行
		manager_connection_->dispose(); //破棄
	}

	bool Task::SystemSetUpAndInitialize(std::unique_ptr<private_::task_manager_connection> &&manager_connection,
	                                    const weak_pointer<i_scene> &pscene) {
		scene_ = pscene;
		manager_connection_ = std::move(manager_connection);
		return OnCreated();
	}

	bool Task::Pause() { return manager_connection_->pause(); }

	bool Task::Resume() { return manager_connection_->resume(); }
}
