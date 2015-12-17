#pragma once

#include "GPMACreateFunctiosDefinition.h"
#include "GPMAKeyPositionFunctionsDefinition.h"

namespace planeta_engine {
	namespace game {
		class GameProcessManagerAccessorForSetUp final
		:public core::GPMACreateFunctionsDefinition,public core::GPMAKeyPositionFunctionsDefinition{
		public:
			explicit GameProcessManagerAccessorForSetUp(GameProcessManagerPublicInterface& gpmpi)
				:GPMACreateFunctionsDefinition(gpmpi), GPMAKeyPositionFunctionsDefinition(gpmpi) {};
		};
	}
}
