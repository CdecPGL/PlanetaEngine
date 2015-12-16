#pragma once

#include "GameProcessListTypes.h"

namespace planeta_engine {
	namespace core {
		//ゲームプロセスリスト内での位置を表す
		struct GameProcessPositionInList final{
			GameProcessListType::iterator pos_in_list;
			GameProcessPriorytyListType::iterator pos_in_priority_list;
		};
	}
}