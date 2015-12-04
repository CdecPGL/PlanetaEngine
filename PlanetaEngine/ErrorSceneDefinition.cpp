#include "ErrorSceneDefinition.h"

#include "SceneAccessorForSetUp.h"
#include "InstanceProcess.h"
#include "SystemLog.h"

namespace planeta_engine {
	namespace core {

		bool ErrorSceneDefinition::SetUpScene(SceneAccessorForSetUp& scene, const utility::ParameterHolder& initialize_parameters) {
			auto ip = scene.game_process_manager().CreateGameProcess<game_processes::InstantProcess>(0);
			ip->SetExcuteFunction([]() {
				debug::SystemLog::instance().LogError("�G���[���������܂����B", __FUNCTION__);
			});
			return true;
		}

	}
}