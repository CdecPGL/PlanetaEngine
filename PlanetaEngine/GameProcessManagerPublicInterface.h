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
				return std::static_pointer_cast<C>(CreateGameProcess([](core::IGameAccessor& ga, int id) {return std::make_shared<C>(ga, id); }, priority));
			}
			/*�Q�[���v���Z�X��ID����擾����*/
			virtual utility::WeakPointer<GameProcess> GetGameProcess(int id) = 0;
			/*�Q�[���v���Z�X��ID�ō폜����*/
			virtual bool RemoveGameProcess(int id) = 0;
			/*�V�X�e���v���Z�X���^����擾����*/
			template<class C>
			utility::WeakPointer<C> GetSystemProcess()const {
				return utility::static_weak_pointer_cast<C>(GetSystemProcess(&utility::CheckSharedPointerTypeOfBase<C, GameProcess>));
			}
			/*�V�X�e���v���Z�X�𖼑O����擾����*/
			virtual utility::WeakPointer<GameProcess> GetSystemProcess(const std::string& name) = 0;
		private:
			virtual std::shared_ptr<GameProcess> CreateGameProcess(const std::function<std::shared_ptr<GameProcess>(core::IGameAccessor&, int)>& creator, int priority) = 0;
			virtual utility::WeakPointer<GameProcess> GetSystemProcess(std::function<bool(const std::shared_ptr<GameProcess>&)> type_checker)const = 0;
		};
	}
}