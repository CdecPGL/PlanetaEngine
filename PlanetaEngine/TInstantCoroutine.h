#pragma once

#include "TCoroutine.h"

namespace planeta_engine {
	class TInstantCoroutine : public CoroutineTask {
	public:
		using CoroutineTask::CoroutineTask;
		using func_type = std::function<void(func_arg_type)>;
		void SetFunction(const func_type& func) { function_ = func; }
	private:
		func_type function_;
		void UpdateProc(func_arg_type yield) { function_(yield); }
	};
	PE_REGISTER_TASK(TInstantCoroutine);
}
