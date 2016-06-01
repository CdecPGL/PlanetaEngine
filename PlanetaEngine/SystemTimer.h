#pragma once
#include "SingletonTemplate.h"
#include "PETime.h"

namespace planeta_engine {
	namespace debug {
		//起動からの経過フレームと経過時間をカウントする(スタティックシングルトン)
		class SystemTimer : public util::SingletonTemplate<SystemTimer> {
			friend util::SingletonTemplate<SystemTimer>;
		public:
			bool Initialize()override;
			void Finalize()override {}

			void IncrementFrameCount() { ++_frame_count; }
			size_t GetCurrentFrameCount()const { return _frame_count; }
			const util::Time GetCurrentTimeCount()const;
		private:
			SystemTimer() = default;
			size_t _frame_count = 0;
			util::Time _start_time;
		};
	}
}
