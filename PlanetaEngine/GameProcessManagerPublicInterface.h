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
			/*�Q�[���v���Z�X���쐬����*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, priority));
			}
			/*�Q�[���v���Z�X���쐬���Ė��O������*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority,const std::string& name) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga) {return std::make_shared<C>(ga); }, priority,const std::string& name));
			}
			/*�Q�[���v���Z�X���^����擾����*/
			template<class C>
			utility::WeakPointer<C> GetProcess()const {
				return utility::static_weak_pointer_cast<C>(GetProcess(&utility::CheckSharedPointerTypeOfBase<C, GameProcess>));
			}
			/*�Q�[���v���Z�X�𖼑O����擾����*/
			virtual utility::WeakPointer<GameProcess> GetProcess(const std::string& name) = 0;
		private:
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, int priority) = 0;
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&)>& creator, int priority,const std::string& name) = 0;
			virtual utility::WeakPointer<GameProcess> GetProcess(std::function<bool(const std::shared_ptr<GameProcess>&)> type_checker)const = 0;
		};
	}
}