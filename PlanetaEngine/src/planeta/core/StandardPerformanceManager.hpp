#pragma once

#include <chrono>
#include "PerfoamanceManager.hpp"

namespace plnt {
	namespace private_ {
		//起動からの経過フレームと経過時間をカウントする
		class StandardPerformanceManager : public PerformanceManager {
		public:
			StandardPerformanceManager() = default;
			bool Initialize() override;
			void Finalize() override;

			void Update() override;
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
