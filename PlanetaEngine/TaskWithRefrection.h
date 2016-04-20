#pragma once

#include "Task.h"
#include "Reflection.h"

/*ñ¢äÆê¨*/
namespace planeta_engine {
	namespace core {
		class IGameAccessor;
	}
	namespace game_processes {
		class TaskWithRefrection : public game::Task,public utility::Reflection{
		public:
			TaskWithRefrection(core::IGameAccessor& g);
		private:
			void ResistRefrectionInformation(RefrectionInformationResistInterface&& riri)override;
			void Update()override;
		};
	}
}
