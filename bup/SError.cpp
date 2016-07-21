#include "SError.h"

#include "SceneDefinitionHeaders.h"
#include "TInstant.h"


namespace planeta {
	namespace core {

		bool SError::SetUpScene(SceneSetUpProxy& scene, const util::ParameterHolder& initialize_parameters) {
			auto ip = scene.CreateTask<TInstant>(TaskSlot::EventUpdatePhase);
			ip->SetExcuteFunction([]() {
				PE_LOG_FATAL("エラーが発生しました。");
			});
			return true;
		}

	}
}