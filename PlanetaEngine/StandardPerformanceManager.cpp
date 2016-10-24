#include "StandardPerformanceManager.h"
#include <ctime>

namespace planeta {
	namespace private_ {

		bool StandardPerformanceManager::Initialize() {
			_frame_count = 0;
			_start_time = util::Time::GetCurrentTime();
			return true;
		}

		const util::Time StandardPerformanceManager::GetCurrentTimeCount() const { return util::Time::GetCurrentTime() - _start_time; }
	}

}