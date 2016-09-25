#include "SEmpty.h"

#include "TInstant.h"

namespace planeta {
	namespace private_ {

		bool SEmpty::SetUpScene(SceneSetUpProxy& scene, const util::ParameterHolder& initialize_parameters) {
			auto ip = scene.CreateTask<TInstant>(TaskSlot::EventUpdatePhase);
			ip->SetExcuteFunction([]() {
				PE_LOG_WARNING("シーンがセットされていません。");
			});
			return true;
		}

	}
}