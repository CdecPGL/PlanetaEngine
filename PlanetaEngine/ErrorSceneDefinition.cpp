#include "ErrorSceneDefinition.h"

#include "SceneDefinitionHeaders.h"
#include "InstanceProcess.h"


namespace planeta_engine {
	namespace core {

		bool ErrorSceneDefinition::SetUpScene(SceneAccessorForSetUp& scene, const utility::ParameterHolder& initialize_parameters) {
			auto ip = scene.CreateGameProcess<game_processes::InstantProcess>(0);
			ip->SetExcuteFunction([]() {
				debug::SystemLog::instance().LogError("�G���[���������܂����B", __FUNCTION__);
			});
			return true;
		}

	}
}