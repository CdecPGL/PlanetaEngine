#pragma once

#include <string>
#include "WeakPointer.h"

namespace planeta_engine {
	namespace core {
		class ScenePublicInterface;
	}
	namespace game {
		class GameObjectSetUpper;
		class GameObjectManagerPublicInterface;
	}
	namespace scene_accessor_impl {
		class SAImplGameObjectManagerSetUpFunctions {
		public:
			explicit SAImplGameObjectManagerSetUpFunctions(core::ScenePublicInterface& spi);
			void RegisterGameObjectSetUpper(const std::string& game_object_create_id, std::shared_ptr<game::GameObjectSetUpper>&& game_object_setupper);
		private:
			game::GameObjectManagerPublicInterface& gompi_;
		};
	}
}