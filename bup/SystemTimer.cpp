#include "SystemTimer.h"
#include <ctime>

namespace planeta {
	namespace core {

	}

	bool debug::SystemTimer::Initialize()
	{
		_frame_count = 0;
		_start_time = util::Time::GetCurrentTime();
		return true;
	}

	const util::Time debug::SystemTimer::GetCurrentTimeCount() const { return util::Time::GetCurrentTime() - _start_time; }

}