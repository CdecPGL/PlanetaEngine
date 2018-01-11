#include "planeta/core/Task.hpp"
#include "TaskManagerConnection.hpp"
#include "planeta/core/IGameObject.hpp"


namespace plnt {
	Task::Task() {}
	Task::~Task() = default;

	void Task::Dispose() {
		disposed(); //イベント実行
		OnDisposed(); //自分のイベント関数実行
		manager_connection_->Dispose(); //破棄
	}

	bool Task::SystemSetUpAndInitialize(std::unique_ptr<private_::TaskManagerConnection>&& manager_connection, const WeakPointer<IScene>& pscene) {
		scene_ = pscene;
		manager_connection_ = std::move(manager_connection);
		return OnCreated();
	}

	bool Task::Pause() {
		return manager_connection_->Pause();
	}

	bool Task::Resume() {
		return manager_connection_->Resume();
	}
}
