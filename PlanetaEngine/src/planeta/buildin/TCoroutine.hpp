#pragma once

#include <functional>
#include <memory>

#include "boost/coroutine2/all.hpp"

#include "planeta/core/task.hpp"

namespace plnt {
	class TCoroutine : public task {
	public:
		using super = task;
		using coroutine = boost::coroutines2::coroutine<void>;
		using func_arg_type = coroutine::push_type &;
		using task::task;
		void Restart() { Start(); }
		/*コルーチンが終了した時にプロセスを破棄するか*/
		void dispose_when_coroutine_finished(bool f) { dispose_when_coroutine_finished_ = f; }

	private:
		bool dispose_when_coroutine_finished_ = true;
		void update() override final;
		virtual void UpdateProc(func_arg_type yield) = 0;
		std::unique_ptr<coroutine::pull_type> coroutine_;
		void Start();
	};

	PE_TASK_CLASS(TCoroutine);
}
