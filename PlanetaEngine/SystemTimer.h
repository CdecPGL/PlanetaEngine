#pragma once
#include "SingletonTemplate.h"
#include "PETime.h"

namespace planeta_engine {
	namespace debug {
		//起動からの経過フレームと経過時間をカウントする(スタティックシングルトン)
		class SystemTimer : public utility::SingletonTemplate<SystemTimer> {
			friend utility::SingletonTemplate<SystemTimer>;
		public:
			bool Initialize()override;
			void Finalize()override {}

			void IncrementFrameCount() { ++_frame_count; }
			size_t GetCurrentFrameCount()const { return _frame_count; }
			const utility::Time GetCurrentTimeCount()const;
		private:
			SystemTimer() = default;
			size_t _frame_count = 0;
			utility::Time _start_time;
		};
	}
}
