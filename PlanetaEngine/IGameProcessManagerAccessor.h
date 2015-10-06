#pragma once

#include <string>
#include <unordered_map>
#include "WeakPointer.h"

namespace planeta_engine {
	namespace core {
		class IGameAccessor;
	};
	namespace game {
		class GameProcess;
		class IGameProcessManagerAccessor {
		public:
			virtual ~IGameProcessManagerAccessor() = 0 {};
			/*�Q�[���v���Z�X���쐬����*/
			template<class C>
			utility::WeakPointer<C> CreateGameProcess(int priority) {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				std::shared_ptr<C> new_proc = std::make_shared<C>(GetGameAccessorReference(),GetNewID());
				if (AddGameProcess(priority, new_proc) >= 0) {
					return new_proc;
				}
				else { return nullptr; }
			}
			/*�Q�[���v���Z�X��ID����擾����*/
			virtual utility::WeakPointer<GameProcess> GetGameProcess(int id) = 0;
			/*�Q�[���v���Z�X��ID�ō폜����*/
			virtual bool RemoveGameProcess(int id) = 0;
			/*�V�X�e���v���Z�X���^����擾����*/
			template<class C>
			utility::WeakPointer<C> GetSystemProcess() {
				static_assert(std::is_base_of<GameProcess, C>::value == true, "C is not derived GameProcess.");
				auto& spl = _GetSystemProcessList();
				for (const auto& p : spl) {
					std::shared_ptr<C> ptr = std::dynamic_pointer_cast<C>(*p.second);
					if (ptr) { return ptr; }
				}
				return nullptr;
			}
			/*�V�X�e���v���Z�X�𖼑O����擾����*/
			virtual utility::WeakPointer<GameProcess> GetSystemProcess(const std::string& name) = 0;
		private:
			virtual std::unordered_map<std::string, std::list<std::shared_ptr<GameProcess>>::iterator>& _GetSystemProcessList() = 0;
			virtual int AddGameProcess(int priority, const std::shared_ptr<GameProcess>& game_process) = 0;
			virtual int GetNewID() = 0;
			virtual core::IGameAccessor& GetGameAccessorReference() = 0;
		};
	}
}
