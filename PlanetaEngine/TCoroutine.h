#pragma once
#include <functional>
#include <memory>
#include "Task.h"
#include "boost/coroutine2/all.hpp"

namespace planeta_engine {

	class TCoroutine :public game::Task {
	public:
		using coroutine = boost::coroutines2::coroutine<void>;
		using func_arg_type = coroutine::push_type&;
		using Task::Task;
		void Restart() { Start(); }
		/*コルーチンが終了した時にプロセスを破棄するか*/
		void dispose_when_coroutine_finished(bool f) { dispose_when_coroutine_finished_ = f; }
	private:
		bool dispose_when_coroutine_finished_ = true;
		void Update()override final;
		virtual void UpdateProc(func_arg_type yield) = 0;
		std::unique_ptr<coroutine::pull_type> coroutine_;
		void Start();
	};
}
