#pragma once

#include "GameProcessManagerPublicInterface.h"
#include "GameProcessPosition.h"

namespace planeta_engine {
	namespace core {
		class GPMAKeyPositionFunctionsDefinition {
		public:
			explicit GPMAKeyPositionFunctionsDefinition(game::GameProcessManagerPublicInterface& pinterface) :game_process_manager_public_interface_(pinterface) {}
			/*キーポジションを作成する*/
			bool CreateKeyPosition(const std::string& id, const core::GameProcessPosition& pos, InsertPosIndication ins_ind) { return game_process_manager_public_interface_.CreateKeyPosition(id, pos, ins_ind); }
			/*キーポジションを取得する*/
			core::GameProcessPosition GetKeyPosition(const std::string& id)const { return game_process_manager_public_interface_.GetKeyPosition(id); }
			/*キーポジションを削除する*/
			bool DeleteKeyPosition(const std::string& id) { return game_process_manager_public_interface_.DeleteKeyPosition(id); }
		private:
			game::GameProcessManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}
