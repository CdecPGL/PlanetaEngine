#pragma once

#include "GameProcessListTypes.h"

namespace planeta_engine {
	namespace core {
		//�Q�[���v���Z�X���X�g���ł̈ʒu��\��
		struct GameProcessPositionInList final{
			GameProcessListType::iterator pos_in_list;
			GameProcessPriorytyListType::iterator pos_in_priority_list;
		};
	}
}