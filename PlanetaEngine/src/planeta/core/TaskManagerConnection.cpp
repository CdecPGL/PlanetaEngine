#include "TaskManagerConnection.hpp"

namespace plnt::private_ {
	bool task_manager_connection::pause() const { return pauser_(); }

	bool task_manager_connection::resume() const { return resumer_(); }

	void task_manager_connection::dispose() const { disposer_(); }
}
