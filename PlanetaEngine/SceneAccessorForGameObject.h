#pragma once

#include "ScenePublicInterface.h"

#include "GameObjectManagerAccessorForGameObject.h"
#include "GameProcessManagerAccessorForGameObject.h"

namespace planeta_engine {
	namespace core {
		class SceneAccessorForGameObject final {
		public:
			explicit SceneAccessorForGameObject(ScenePublicInterface& spi) :game_object_manager_accessor_(spi.game_object_manager()), game_process_manager_accessor_(spi.game_process_manager()){}
			game::GameObjectManagerAccessorForGameObject& game_object_manager() { return game_object_manager_accessor_; }
			game::GameProcessManagerAccessorForGameObject& game_process_manager() { return game_process_manager_accessor_; }
		private:
			game::GameObjectManagerAccessorForGameObject game_object_manager_accessor_;
			game::GameProcessManagerAccessorForGameObject game_process_manager_accessor_;
		};
	}
}