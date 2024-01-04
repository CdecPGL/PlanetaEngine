#pragma once

#include "t_coroutine.hpp"

namespace plnt {
	class t_instant_coroutine final : public t_coroutine {
	public:
		using super = t_coroutine;
		using t_coroutine::t_coroutine;
		using func_type = std::function<void(func_arg_type)>;
		void set_function(const func_type &func) { function_ = func; }

	private:
		func_type function_;
		void update_proc(func_arg_type yield) override { function_(yield); }
	};

	PE_TASK_CLASS(t_instant_coroutine);
}
