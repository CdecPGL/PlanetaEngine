#pragma once

#include "ScenePublicInterface.h"
#include "TaskManagerPublicInterface.h"

namespace planeta_engine {
	namespace scene_accessor_impl {
		class SAImplTaskGetFunctions {
		public:
			explicit SAImplTaskGetFunctions(core::ScenePublicInterface& spi) :game_process_manager_public_interface_(spi.game_process_manager()) {}
			/*�Q�[���v���Z�X���^����擾����*/
			template<class C>
			utility::WeakPointer<C> GetTask()const { return game_process_manager_public_interface_.GetProcess<C>(); }
			/*�Q�[���v���Z�X�𖼑O����擾����*/
			utility::WeakPointer<game::Task> GetTask(const std::string& name) { return game_process_manager_public_interface_.GetProcess(name); }
		private:
			game::TaskManagerPublicInterface& game_process_manager_public_interface_;
		};
	}
}