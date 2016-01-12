#include "SAImplGameProcessKeyPositionFunctions.h"
#include "ScenePublicInterface.h"
#include "GameProcessManagerPublicInterface.h"

namespace planeta_engine {
	namespace scene_accessor_impl {

		SAImplGameProcessKeyPositionFunctions::SAImplGameProcessKeyPositionFunctions(core::ScenePublicInterface& spi) :gpmpi_(spi.game_process_manager()) {}

		core::GameProcessPosition SAImplGameProcessKeyPositionFunctions::GetKeyPosition(const std::string& id) const {
			return gpmpi_.GetKeyPosition(id);
		}

	}
}