#pragma once

#include "GameProcessManagerPublicInterface.h"
#include "GameProcessPosition.h"

namespace planeta_engine {
	namespace core {
		class GPMAKeyPositionFunctionsDefinition {
		public:
			explicit GPMAKeyPositionFunctionsDefinition(game::GameProcessManagerPublicInterface& pinterface) :game_process_manager_public_interface_(pinterface) {}
			/*キーポジションを取得する*/
			core::GameProcessPosition GetKeyPosition(const std::string& id)const { return game_process_manager_public_interface_.GetKeyPosition(id); }
		private:
			game::GameProcessManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}
