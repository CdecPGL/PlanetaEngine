#include "SystemCounter.h"
#include <ctime>

namespace planeta_engine {
	namespace core {

	}

	bool debug::SystemCounter::Initialize()
	{
		_frame_count = 0;
		_start_time = utility::Time::GetCurrentTime();
		return true;
	}
}