#include "ErrorSceneDefinition.h"

#include "SceneDefinitionHeaders.h"
#include "TInstant.h"


namespace planeta_engine {
	namespace core {

		bool ErrorSceneDefinition::SetUpScene(SceneAccessorForSetUp& scene, const utility::ParameterHolder& initialize_parameters) {
			auto ip = scene.CreateTask<game_processes::TInstant>(0);
			ip->SetExcuteFunction([]() {
				debug::SystemLog::instance().LogError("ÉGÉâÅ[Ç™î≠ê∂ÇµÇ‹ÇµÇΩÅB", __FUNCTION__);
			});
			return true;
		}

	}
}