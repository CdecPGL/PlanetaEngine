#include "TaskWithRefrection.h"

namespace planeta_engine {
	TaskWithRefrection::TaskWithRefrection(core::IGameAccessor& g) :Task(g) {
		SetUpRefrection(this);
	}

	void TaskWithRefrection::ResistRefrectionInformation(RefrectionInformationResistInterface&& riri) {

	}

	void TaskWithRefrection::Update() {

	}
}