#pragma once

#include <chrono>
#include "PerfoamanceManager.h"

namespace planeta {
	namespace private_ {
		//起動からの経過フレームと経過時間をカウントする
		class StandardPerformanceManager : public PerformanceManager {
		public:
			StandardPerformanceManager() = default;
			bool Initialize()override;
			void Finalize()override;

			void Update()override;
			size_t GetCurrentFrameCount()const override { return _frame_count; }
			const util::Time GetCurrentTimeCount()const override;
			void DebugInfotmationAddHandler(IDebugInformationAdder& di_adder) override;
		private:
			size_t _frame_count = 0;
			util::Time _start_time;
			std::chrono::system_clock::time_point last_measure_fps_time_;
			int fps_measure_count_ = 0;
			double fps_ = 0;
		};
	}
}
