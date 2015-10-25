#pragma once

#include "CoroutineProcess.h"

namespace planeta_engine {
	namespace game_processes {
		class InstantCoroutineProcess : public CoroutineProcess {
		public:
			using CoroutineProcess::CoroutineProcess;
			void SetFunction(const std::function<void(func_arg_type)>& func) { function_ = func; }
		private:
			std::function<void(func_arg_type)> function_;
			void UpdateProc(func_arg_type yield) { function_(yield); }
		};
	}
}
