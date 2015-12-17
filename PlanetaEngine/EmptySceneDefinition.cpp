#include "EmptySceneDefinition.h"

#include "SceneAccessorForSetUp.h"
#include "InstanceProcess.h"
#include "SystemLog.h"
#include "GameProcessSystemKeyPositions.h"

namespace planeta_engine {
	namespace core {

		bool EmptySceneDefinition::SetUpScene(SceneAccessorForSetUp& scene, const utility::ParameterHolder& initialize_parameters) {
			auto ip = scene.game_process_manager().CreateGameProcess<game_processes::InstantProcess>(std::string(gameprocess_sys_key_pos::GameObjectUpdatetProcessKeyPositionID),InsertPosIndication::At);
//			auto ip = scene.game_process_manager().CreateGameProcess<int>(core::GameProcessPosition(), InsertPosIndication::At);
			ip->SetExcuteFunction([]() {
				debug::SystemLog::instance().LogWarning("シーンがセットされていません。", __FUNCTION__);
			});
			return true;
		}

	}
}