#include "task.hpp"
#include "task_manager_connection.hpp"
#include "i_game_object.hpp"


namespace plnt {
	task::task() { }

	task::~task() = default;

	void task::dispose() {
		disposed(); //イベント実行
		on_disposed(); //自分のイベント関数実行
		manager_connection_->dispose(); //破棄
	}

	bool task::system_set_up_and_initialize(std::unique_ptr<private_::task_manager_connection> &&manager_connection,
	                                    const weak_pointer<i_scene> &p_scene) {
		scene_ = p_scene;
		manager_connection_ = std::move(manager_connection);
		return on_created();
	}

	bool task::pause() const { return manager_connection_->pause(); }

	bool task::resume() const { return manager_connection_->resume(); }
}
