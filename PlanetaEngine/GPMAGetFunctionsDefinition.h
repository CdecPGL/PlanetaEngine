#pragma once

#include "GameProcessManagerPublicInterface.h"

namespace planeta_engine {
	namespace core {
		class GPMAGetFunctionsDefinition {
		public:
			explicit GPMAGetFunctionsDefinition(game::GameProcessManagerPublicInterface& pinterface) :game_process_manager_public_interface_(pinterface) {}
			/*�Q�[���v���Z�X���^����擾����*/
			template<class C>
			utility::WeakPointer<C> GetGameProcess()const { return game_process_manager_public_interface_.GetProcess<C>(); }
			/*�Q�[���v���Z�X�𖼑O����擾����*/
			utility::WeakPointer<game::GameProcess> GetGameProcess(const std::string& name) { return game_process_manager_public_interface_.GetProcess(name); }
		private:
			game::GameProcessManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}