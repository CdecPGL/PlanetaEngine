#pragma once

#include "CoroutineTask.h"

namespace planeta_engine {
	namespace game_processes {
		class InstantCoroutineTask : public CoroutineProcess {
		public:
			using CoroutineProcess::CoroutineProcess;
			using func_type = std::function<void(func_arg_type)>;
			void SetFunction(const func_type& func) { function_ = func; }
		private:
			func_type function_;
			void UpdateProc(func_arg_type yield) { function_(yield); }
		};
	}
}
