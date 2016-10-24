#pragma once

#include "PerfoamanceManager.h"

namespace planeta {
	namespace private_ {
		//起動からの経過フレームと経過時間をカウントする
		class StandardPerformanceManager : public PerformanceManager {
		public:
			StandardPerformanceManager() = default;
			bool Initialize()override;
			void Finalize()override {}

			void Update()override { ++_frame_count; }
			size_t GetCurrentFrameCount()const override { return _frame_count; }
			const util::Time GetCurrentTimeCount()const override;
		private:
			size_t _frame_count = 0;
			util::Time _start_time;
		};
	}
}
