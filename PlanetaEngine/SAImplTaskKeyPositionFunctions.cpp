#include "SAImplTaskKeyPositionFunctions.h"
#include "ScenePublicInterface.h"
#include "TaskManagerPublicInterface.h"

namespace planeta_engine {
	namespace scene_accessor_impl {

		SAImplTaskKeyPositionFunctions::SAImplTaskKeyPositionFunctions(core::ScenePublicInterface& spi) :gpmpi_(spi.game_process_manager()) {}

		core::TaskPosition SAImplTaskKeyPositionFunctions::GetKeyPosition(const std::string& id) const {
			return gpmpi_.GetKeyPosition(id);
		}

	}
}