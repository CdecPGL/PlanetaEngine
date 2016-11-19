#include "SError.h"
#include "LogUtility.h"
#include "TInstant.h"
#include "IScene.h"
#include "ITaskManager.h"
#include "SceneSetUpProxy.h"


namespace planeta {
	namespace private_ {

		bool SError::SetUpScene(SceneSetUpProxy& scene_proxy, const util::ParameterHolder& initialize_parameters) {
			auto ip = scene_proxy.scene().task_manager().CreateTask<TInstant>(TaskSlot::EventUpdatePhase);
			ip->SetExcuteFunction([]() {
				PE_LOG_FATAL("エラーが発生しました。");
			});
			return true;
		}

	}
}