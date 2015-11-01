#include "GameProcessWithRefrection.h"

namespace planeta_engine {
	namespace game_processes {

		GameProcessWithRefrection::GameProcessWithRefrection(core::IGameAccessor& g, int id) :GameProcess(g, id)
		{
			SetUpRefrection(this);
		}

		void GameProcessWithRefrection::ResistRefrectionInformation(RefrectionInformationResistInterface& riri)
		{

		}

		void GameProcessWithRefrection::Update()
		{

		}

	}
}