#pragma once

#include <memory>
#include <functional>
#include "WeakPointer.h"
#include "TypeChecker.h"
#include "InsertPosIndication.h"

namespace planeta_engine {
	namespace core {
		class IGameAccessor;
		class GameProcessPositionInList;
	}
	namespace game {
		class GameProcess;
		class GameProcessManagerPublicInterface {
		public:
			virtual ~GameProcessManagerPublicInterface() = default;
			/*ゲームプロセスを作成する*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const core::GameProcessPositionInList& pos,InsertPosIndication pos_indication) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, priority));
			}
			/*ゲームプロセスを作成して名前をつける*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(const std::string& name,const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, priority,const std::string& name));
			}
			/*ゲームプロセスを名前から取得する*/
			virtual utility::WeakPointer<GameProcess> GetProcess(const std::string& name) = 0;
		private:
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication) = 0;
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, const core::GameProcessPositionInList& pos, InsertPosIndication pos_indication,const std::string& name) = 0;
		};
	}
}