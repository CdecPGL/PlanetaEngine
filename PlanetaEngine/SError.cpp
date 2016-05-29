#include "SError.h"

#include "SceneDefinitionHeaders.h"
#include "TInstant.h"


namespace planeta_engine {
	namespace core {

		bool SError::SetUpScene(SceneSetUpProxy& scene, const utility::ParameterHolder& initialize_parameters) {
			auto ip = scene.CreateTask<TInstant>(TaskSlot::EventUpdatePhase);
			ip->SetExcuteFunction([]() {
				PE_LOG_FATAL("ÉGÉâÅ[Ç™î≠ê∂ÇµÇ‹ÇµÇΩÅB");
			});
			return true;
		}

	}
}