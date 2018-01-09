#include "TaskManagerConnection.hpp"

namespace planeta {
	namespace private_ {
		bool TaskManagerConnection::Pause() {
			return pauser_();
		}

		bool TaskManagerConnection::Resume() {
			return resumer_();
		}

		void TaskManagerConnection::Dispose() {
			disposer_();
		}
	}
}