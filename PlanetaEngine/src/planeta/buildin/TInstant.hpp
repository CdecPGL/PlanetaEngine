#pragma once

#include <functional>

#include "planeta/core/Task.hpp"

namespace plnt {
	class TInstant final : public Task {
	public:
		using super = Task;
		using Task::Task;
		void SetExcuteFunction(const std::function<void()> &func) { function_ = func; }
		void number_of_times_implemented(unsigned int n) { number_of_times_implemented_ = n; }

	private:
		std::function<void()> function_;
		void Update() override;
		unsigned int number_of_times_implemented_ = 0; //実行回数(0で無限)
		unsigned int count_of_times_implemented_; //実行回数カウンタ
	};

	PE_TASK_CLASS(TInstant);
}
