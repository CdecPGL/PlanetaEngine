#pragma once

#include "ScenePublicInterface.h"

#include "GameObjectManagerAccessorForGameProcess.h"
#include "GUIManagerAccessorForGameProcess.h"
#include "GameProcessManagerAccessorForGameProcess.h"

namespace planeta_engine {
	namespace core {
		class SceneAccessorForGameProcess final{
		public:
			explicit SceneAccessorForGameProcess(ScenePublicInterface& spi) :game_object_manager_accessor_(spi.game_object_manager()),game_process_manager_accessor_(spi.game_process_manager()),gui_manager_accessor_(spi.gui_manager()) {}
			game::GameObjectManagerAccessorForGameProcess& game_object_manager() { return game_object_manager_accessor_; }
			game::GUIManagerAccessorForGameProcess& gui_manager() { return gui_manager_accessor_; }
			game::GameProcessManagerAccessorForGameProcess& game_process_manager() { return game_process_manager_accessor_; }
		private:
			game::GameObjectManagerAccessorForGameProcess game_object_manager_accessor_;
			game::GUIManagerAccessorForGameProcess gui_manager_accessor_;
			game::GameProcessManagerAccessorForGameProcess game_process_manager_accessor_;
		};
	}
}
