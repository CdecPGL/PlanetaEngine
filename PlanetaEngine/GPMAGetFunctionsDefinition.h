#pragma once

#include "GameProcessManagerPublicInterface.h"

namespace planeta_engine {
	namespace core {
		class GPMAGetFunctionsDefinition {
		public:
			explicit GPMAGetFunctionsDefinition(game::GameProcessManagerPublicInterface& pinterface) :game_process_manager_public_interface_(pinterface) {}
			/*ゲームプロセスを型から取得する*/
			template<class C>
			utility::WeakPointer<C> GetGameProcess()const { return game_process_manager_public_interface_.GetProcess<C>(); }
			/*ゲームプロセスを名前から取得する*/
			utility::WeakPointer<game::GameProcess> GetGameProcess(const std::string& name) { return game_process_manager_public_interface_.GetProcess(name); }
		private:
			game::GameProcessManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}