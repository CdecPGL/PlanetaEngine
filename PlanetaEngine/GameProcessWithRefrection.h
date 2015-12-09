#pragma once

#include "GameProcess.h"
#include "Reflection.h"

/*ñ¢äÆê¨*/
namespace planeta_engine {
	namespace core {
		class IGameAccessor;
	}
	namespace game_processes {
		class GameProcessWithRefrection : public game::GameProcess,public utility::Reflection{
		public:
			GameProcessWithRefrection(core::IGameAccessor& g,int id);
		private:
			void ResistRefrectionInformation(RefrectionInformationResistInterface&& riri)override;
			void Update()override;
		};
	}
}
