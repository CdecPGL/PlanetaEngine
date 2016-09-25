#include "SError.h"

#include "TInstant.h"


namespace planeta {
	namespace private_ {

		bool SError::SetUpScene(SceneSetUpProxy& scene, const util::ParameterHolder& initialize_parameters) {
			auto ip = scene.CreateTask<TInstant>(TaskSlot::EventUpdatePhase);
			ip->SetExcuteFunction([]() {
				PE_LOG_FATAL("エラーが発生しました。");
			});
			return true;
		}

	}
}