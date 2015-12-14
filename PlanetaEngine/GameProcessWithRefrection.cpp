#include "GameProcessWithRefrection.h"

namespace planeta_engine {
	namespace game_processes {

		GameProcessWithRefrection::GameProcessWithRefrection(core::IGameAccessor& g) :GameProcess(g)
		{
			SetUpRefrection(this);
		}

		void GameProcessWithRefrection::ResistRefrectionInformation(RefrectionInformationResistInterface&& riri)
		{

		}

		void GameProcessWithRefrection::Update()
		{

		}

	}
}