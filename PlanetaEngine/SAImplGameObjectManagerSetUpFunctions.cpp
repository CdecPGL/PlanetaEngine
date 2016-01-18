#include "SAImplGameObjectManagerSetUpFunctions.h"
#include "ScenePublicInterface.h"
#include "GameObjectManagerPublicInterface.h"

namespace planeta_engine {
	namespace scene_accessor_impl {

		SAImplGameObjectManagerSetUpFunctions::SAImplGameObjectManagerSetUpFunctions(core::ScenePublicInterface& spi) :gompi_(spi.game_object_manager()) {}

		void SAImplGameObjectManagerSetUpFunctions::RegisterGameObjectSetUpper(const std::string& game_object_create_id, std::shared_ptr<game::GameObjectSetUpper>&& game_object_setupper) {
			gompi_.RegisterGameObjectSetUpper(game_object_create_id, std::move(game_object_setupper));
		}

	}
}