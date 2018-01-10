#include "PETime.hpp"
#include <sstream>

namespace plnt {
	namespace util {

		std::string Time::ToString() const
		{
			std::stringstream sstrm;
			sstrm << _hour << ':' << _minute << ':' << _second;
			return std::move(sstrm.str());
		}

		Time Time::GetCurrentTime() {
			time_t now = time(nullptr);
			struct tm pnow;
			localtime_s(&pnow, &now);
			return Time(pnow);
		}

	}
}