#include "SystemTimer.h"
#include <ctime>

namespace planeta_engine {
	namespace core {

	}

	bool debug::SystemTimer::Initialize()
	{
		_frame_count = 0;
		_start_time = utility::Time::GetCurrentTime();
		return true;
	}

	const utility::Time debug::SystemTimer::GetCurrentTimeCount() const { return utility::Time::GetCurrentTime() - _start_time; }

}