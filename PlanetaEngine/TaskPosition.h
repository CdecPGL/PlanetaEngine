#pragma once

#include "TaskListTypes.h"

namespace planeta_engine {
	namespace core {
		//�Q�[���v���Z�X���X�g���ł̈ʒu��\��
		struct TaskPosition {
			TaskPosition() {}
			TaskPosition(TaskPriorytyListType::iterator pl_it) :priority_list_iterator(pl_it) {}
			TaskPriorytyListType::iterator priority_list_iterator;
		};
	}
}