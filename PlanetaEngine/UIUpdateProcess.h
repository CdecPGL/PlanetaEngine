#pragma once
#include "GameProcess.h"

namespace planeta_engine {
	namespace system_processes {
		class UIUpdateProcess :
			public game::GameProcess
		{
		public:
			GameProcess::GameProcess;
			~UIUpdateProcess();
		private:
			void Update()override final;
		};
	}
}
