#include "TaskManagerConnection.h"

namespace planeta_engine {
	namespace core {
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