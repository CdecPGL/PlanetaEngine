#include "ErrorSceneDefinition.h"

#include "SceneDefinitionHeaders.h"
#include "InstantProcess.h"


namespace planeta_engine {
	namespace core {

		bool ErrorSceneDefinition::SetUpScene(SceneAccessorForSetUp& scene, const utility::ParameterHolder& initialize_parameters) {
			auto ip = scene.CreateTask<game_processes::InstantProcess>(0);
			ip->SetExcuteFunction([]() {
				debug::SystemLog::instance().LogError("ÉGÉâÅ[Ç™î≠ê∂ÇµÇ‹ÇµÇΩÅB", __FUNCTION__);
			});
			return true;
		}

	}
}