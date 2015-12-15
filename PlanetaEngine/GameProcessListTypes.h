#pragma once

#include <list>
#include <memory>

namespace planeta_engine {
	namespace game {
		class GameProcess;
	}
	namespace core {
		struct GameProcessPositionInList;
		using GameProcessListType = std::list<std::shared_ptr<game::GameProcess>>;
		using GameProcessPriorytyListType = std::list<GameProcessListType>;
		using GameProcessPositionTypeAtList = GameProcessPositionInList;
	}
}