#include "PEDateTime.h"
#include <sstream>

namespace planeta_engine {
	namespace util {

		std::string DateTime::ToString() const
		{
			std::stringstream sstrm;
			sstrm << _year << '/' << _month << '/' << _day << ' ' << _hour << ':' << _minute << ':' << _second;
			return std::move(sstrm.str());
		}

		DateTime DateTime::GetCurrentDateTime() {
			time_t now = time(nullptr);
			struct tm pnow;
			localtime_s(&pnow, &now);
			return DateTime(pnow);
		}

	}
}