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
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, priority));
			}
			/*ゲームプロセスを作成して名前をつける*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority,const std::string& name) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, priority,const std::string& name));
			}
			/*ゲームプロセスを型から取得する*/
			template<class C>
			utility::WeakPointer<C> GetProcess()const {
				return utility::static_weak_pointer_cast<C>(GetProcess(&utility::CheckSharedPointerTypeOfBase<C, GameProcess>));
			}
			/*ゲームプロセスを名前から取得する*/
			virtual utility::WeakPointer<GameProcess> GetProcess(const std::string& name) = 0;
		private:
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, int priority) = 0;
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, int priority,const std::string& name) = 0;
			virtual utility::WeakPointer<GameProcess> GetProcess(std::function<bool(const std::shared_ptr<GameProcess>&)> type_checker)const = 0;
		};
	}
}