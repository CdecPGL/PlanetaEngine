#include "EmptySceneDefinition.h"

#include "SceneDefinitionHeaders.h"
#include "TInstant.h"

namespace planeta_engine {
	namespace core {

		bool EmptySceneDefinition::SetUpScene(SceneAccessorForSetUp& scene, const utility::ParameterHolder& initialize_parameters) {
			auto ip = scene.CreateTask<game_processes::TInstant>(std::string(task_sys_key_pos::GameObjectUpdatetTaskKeyPositionID));
//			auto ip = scene.game_process_manager().CreateTask<int>(core::TaskPosition(), InsertPosIndication::At);
			ip->SetExcuteFunction([]() {
				debug::SystemLog::instance().LogWarning("シーンがセットされていません。", __FUNCTION__);
			});
			return true;
		}

	}
}