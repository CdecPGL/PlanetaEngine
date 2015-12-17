#pragma once

#include "GPMACreateFunctiosDefinition.h"
#include "GPMAGetFunctionsDefinition.h"

namespace planeta_engine {
	namespace game {
		class GameProcessManagerAccessorForGameObject final 
		:public core::GPMACreateFunctionsDefinition,public core::GPMAGetFunctionsDefinition{
		public:
			explicit GameProcessManagerAccessorForGameObject(GameProcessManagerPublicInterface& gpmpi)
				:GPMACreateFunctionsDefinition(gpmpi), GPMAGetFunctionsDefinition(gpmpi) {};
		};
	}
}
