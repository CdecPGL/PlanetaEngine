#pragma once

#include "ScenePublicInterface.h"

#include "GameObjectManagerAccessorForSetUp.h"
#include "GUIManagerAccessorForSetUp.h"
#include "GameProcessManagerAccessorForSetUp.h"

namespace planeta_engine {
	namespace core {
		class SceneAccessorForSetUp final {
		public:
			explicit SceneAccessorForSetUp(ScenePublicInterface& spi) :game_object_manager_accessor_(spi.game_object_manager()), game_process_manager_accessor_(spi.game_process_manager()), gui_manager_accessor_(spi.gui_manager()) {}
			game::GameObjectManagerAccessorForSetUp& game_object_manager() { return game_object_manager_accessor_; }
			game::GUIManagerAccessorForSetUp& gui_manager() { return gui_manager_accessor_; }
			game::GameProcessManagerAccessorForSetUp& game_process_manager() { return game_process_manager_accessor_; }

		private:
			game::GameObjectManagerAccessorForSetUp game_object_manager_accessor_;
			game::GUIManagerAccessorForSetUp gui_manager_accessor_;
			game::GameProcessManagerAccessorForSetUp game_process_manager_accessor_;
		};
	}
}
