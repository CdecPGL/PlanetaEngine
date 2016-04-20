#pragma once

#include "TaskListTypes.h"

namespace planeta_engine {
	namespace core {
		//ゲームプロセスリスト内での位置を表す
		struct TaskPosition {
			TaskPosition() {}
			TaskPosition(TaskPriorytyListType::iterator pl_it) :priority_list_iterator(pl_it) {}
			TaskPriorytyListType::iterator priority_list_iterator;
		};
	}
}