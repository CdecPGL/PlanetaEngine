#pragma once
#include "GameProcess.h"

namespace planeta_engine{
	namespace system_processes {
		class UIDrawProcess :
			public game::GameProcess
		{
		public:
			GameProcess::GameProcess;
			~UIDrawProcess();
		private:
			void Update()override final;
		};
	}
}

