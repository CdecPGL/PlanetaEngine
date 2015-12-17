#pragma once

#include <list>
#include <memory>

namespace planeta_engine {
	namespace game {
		class GameProcess;
	}
	namespace core {
		using GameProcessListType = std::list<std::shared_ptr<game::GameProcess>>;
		using GameProcessPriorytyListType = std::list<GameProcessListType>;
	}
}