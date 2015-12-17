#pragma once

#include "GameProcessManagerPublicInterface.h"
#include "GameProcessPosition.h"

namespace planeta_engine {
	namespace core {
		class GPMAKeyPositionFunctionsDefinition {
		public:
			explicit GPMAKeyPositionFunctionsDefinition(game::GameProcessManagerPublicInterface& pinterface) :game_process_manager_public_interface_(pinterface) {}
			/*�L�[�|�W�V�������쐬����*/
			bool CreateKeyPosition(const std::string& id, const core::GameProcessPosition& pos, InsertPosIndication ins_ind) { return game_process_manager_public_interface_.CreateKeyPosition(id, pos, ins_ind); }
			/*�L�[�|�W�V�������擾����*/
			core::GameProcessPosition GetKeyPosition(const std::string& id)const { return game_process_manager_public_interface_.GetKeyPosition(id); }
			/*�L�[�|�W�V�������폜����*/
			bool DeleteKeyPosition(const std::string& id) { return game_process_manager_public_interface_.DeleteKeyPosition(id); }
		private:
			game::GameProcessManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}
