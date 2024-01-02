#include <sstream>

#include "time.hpp"

namespace plnt::util {
	std::string time::to_string() const {
		std::stringstream sstream;
		sstream << hour_ << ':' << minute_ << ':' << second_;
		return sstream.str();
	}

	time time::get_current_time() {
		const time_t now = ::time(nullptr);
		tm p_now;
		if (localtime_s(&p_now, &now) != 0) { throw std::runtime_error("Failed to get current time."); }
		return time(p_now);
	}
}
