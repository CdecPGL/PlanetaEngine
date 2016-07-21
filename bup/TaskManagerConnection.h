#pragma once

#include <functional>
#include "WeakPointer.h"

namespace planeta {
	namespace core {
		class TaskManagerConnection {
		public:
			TaskManagerConnection(std::function<bool()>&& pauser, std::function<bool()>&& resumer, std::function<void()>&& disposer)
				:pauser_(pauser), resumer_(resumer), disposer_(disposer) {};
			bool Pause();
			bool Resume();
			void Dispose();
		private:
			std::function<bool()> pauser_;
			std::function<bool()> resumer_;
			std::function<void()> disposer_;
		};
	}
}