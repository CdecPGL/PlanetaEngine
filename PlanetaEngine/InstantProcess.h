#pragma once

#include "Task.h"
#include <functional>

namespace planeta_engine {
	namespace game_processes {
		class InstantProcess final: public game::Task{
		public:
			using Task::Task;
			void SetExcuteFunction(const std::function<void()>& func) { function_ = func; }
			void number_of_times_implemented(unsigned int n) { number_of_times_implemented_ = n; }
		private:
			std::function<void()> function_;
			void Update() override;
			unsigned int number_of_times_implemented_ = 0; //実行回数(0で無限)
			unsigned int count_of_times_implemented_; //実行回数カウンタ
		};
	}
}

