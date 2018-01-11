#include "SError.hpp"
#include "planeta/core/LogUtility.hpp"
#include "TInstant.hpp"
#include "planeta/core/IScene.hpp"
#include "planeta/core/ITaskManager.hpp"
#include "planeta/core/SceneSetUpProxy.hpp"


namespace plnt {
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