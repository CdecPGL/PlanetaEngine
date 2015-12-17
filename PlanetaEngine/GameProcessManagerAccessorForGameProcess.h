#pragma once

#include "GPMACreateFunctiosDefinition.h"
#include "GPMAGetFunctionsDefinition.h"

namespace planeta_engine {
	namespace game {
		class GameProcessManagerAccessorForGameProcess final 
		:public core::GPMACreateFunctionsDefinition,public core::GPMAGetFunctionsDefinition{
		public:
			explicit GameProcessManagerAccessorForGameProcess(GameProcessManagerPublicInterface& gpmpi)
				:GPMACreateFunctionsDefinition(gpmpi), GPMAGetFunctionsDefinition(gpmpi) {};
		private:
		};
	}
}