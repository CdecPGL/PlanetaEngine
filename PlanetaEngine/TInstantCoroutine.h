#pragma once

#include "TCoroutine.h"

namespace planeta {
	class TInstantCoroutine : public TCoroutine {
	public:
		using TCoroutine::TCoroutine;
		using func_type = std::function<void(func_arg_type)>;
		void SetFunction(const func_type& func) { function_ = func; }
	private:
		func_type function_;
		void UpdateProc(func_arg_type yield) { function_(yield); }
	};
	PE_TASK_CLASS(TInstantCoroutine);
}
