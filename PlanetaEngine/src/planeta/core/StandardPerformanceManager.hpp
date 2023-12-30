#pragma once

#include <chrono>
#include "perfoamance_manager.hpp"

namespace plnt {
	namespace private_ {
		//起動からの経過フレームと経過時間をカウントする
		class StandardPerformanceManager : public performance_manager {
		public:
			StandardPerformanceManager() = default;
			bool initialize() override;
			void finalize() override;

			void update() override;
			size_t get_current_frame_count() const override { return _frame_count; }
			const util::Time get_current_time_count() const override;
			void DebugInfotmationAddHandler(i_debug_information_adder &di_adder) override;

		private:
			size_t _frame_count = 0;
			util::Time _start_time;
			std::chrono::system_clock::time_point last_measure_fps_time_;
			int fps_measure_count_ = 0;
			double fps_ = 0;
		};
	}
}
