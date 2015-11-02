#pragma once

#include <memory>
#include <functional>
#include "WeakPointer.h"
#include "TypeChecker.h"

namespace planeta_engine {
	namespace core {
		class IGameAccessor;
	}
	namespace game {
		class GameProcess;
		class GameProcessManagerPublicInterface {
		public:
			virtual ~GameProcessManagerPublicInterface() = default;
			/*ゲームプロセスを作成する*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga, int id) {return std::make_shared<C>(ga, id); }, priority));
			}
			/*ゲームプロセスをIDから取得する*/
			virtual utility::WeakPointer<GameProcess> GetGameProcess(int id) = 0;
			/*ゲームプロセスをIDで削除する*/
			virtual bool RemoveGameProcess(int id) = 0;
			/*システムプロセスを型から取得する*/
			template<class C>
			utility::WeakPointer<C> GetSystemProcess()const {
				return utility::static_weak_pointer_cast<C>(GetSystemProcess(&utility::CheckSharedPointerTypeOfBase<C, GameProcess>));
			}
			/*システムプロセスを名前から取得する*/
			virtual utility::WeakPointer<GameProcess> GetSystemProcess(const std::string& name) = 0;
		private:
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&, int)>& creator, int priority) = 0;
			virtual utility::WeakPointer<GameProcess> GetSystemProcess(std::function<bool(const std::shared_ptr<GameProcess>&)> type_checker)const = 0;
		};
	}
}