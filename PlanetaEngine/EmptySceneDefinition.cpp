#include "EmptySceneDefinition.h"

#include "SceneDefinitionHeaders.h"
#include "TInstant.h"

namespace planeta_engine {
	namespace core {

		bool EmptySceneDefinition::SetUpScene(SceneSetUpProxy& scene, const utility::ParameterHolder& initialize_parameters) {
			auto ip = scene.CreateTask<TInstant>(TaskSlot::EventUpdatePhase);
			ip->SetExcuteFunction([]() {
				PE_LOG_WARNING("シーンがセットされていません。");
			});
			return true;
		}

	}
}