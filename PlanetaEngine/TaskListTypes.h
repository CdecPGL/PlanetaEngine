#pragma once

#include <list>
#include <memory>

namespace planeta_engine {
	namespace game {
		class Task;
	}
	namespace core {
		using TaskListType = std::list<std::shared_ptr<game::Task>>;
		using TaskPriorytyListType = std::list<TaskListType>;
	}
}