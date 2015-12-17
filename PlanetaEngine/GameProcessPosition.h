#pragma once

#include "GameProcessListTypes.h"

namespace planeta_engine {
	namespace core {
		//�Q�[���v���Z�X���X�g���ł̈ʒu��\��
		struct GameProcessPosition {
			GameProcessPosition();
			GameProcessPosition(GameProcessPriorytyListType::iterator pl_it) :priority_list_iterator(pl_it) {}
			GameProcessPriorytyListType::iterator priority_list_iterator;
		};
	}
}