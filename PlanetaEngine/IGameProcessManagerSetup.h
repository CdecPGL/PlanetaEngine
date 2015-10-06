#pragma once

#pragma once

#include <string>
#include <memory>
#include "WeakPointer.h"

namespace planeta_engine {
	namespace core {
		class IGameAccessor;
	};
	namespace game {
		class GameProcess;
		class IGameProcessManagerSetup {
		public:
			virtual ~IGameProcessManagerSetup() = 0 {};
			/*ゲームプロセスを作成する*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				std::shared_ptr<C> new_proc = std::make_shared<C>(GetGameAccessorReference(),GetNewID());
				if (AddGameProcess(priority, new_proc) >= 0) {
					return new_proc;
				}
				else { return nullptr; }
			}
		private:
			virtual int AddGameProcess(int priority, const std::shared_ptr<GameProcess>& game_process) = 0;
			virtual int GetNewID() = 0;
			virtual core::IGameAccessor& GetGameAccessorReference() = 0;
		};
	}
}
