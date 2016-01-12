#pragma once

#include "ScenePublicInterface.h"
#include "GameProcessManagerPublicInterface.h"

namespace planeta_engine {
	namespace scene_accessor_impl {
		class SAImplGameProcessGetFunctions {
		public:
			explicit SAImplGameProcessGetFunctions(core::ScenePublicInterface& spi) :game_process_manager_public_interface_(spi.game_process_manager()) {}
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