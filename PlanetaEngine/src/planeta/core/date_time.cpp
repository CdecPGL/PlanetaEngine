#include <sstream>

#include "date_time.hpp"

namespace plnt::util {
	std::string date_time::to_string() const {
		std::stringstream sst;
		sst << year_ << '/' << month_ << '/' << day_ << ' ' << hour_ << ':' << minute_ << ':' << second_;
		return sst.str();
	}

	date_time date_time::get_current_date_time() {
		const time_t now = time(nullptr);
		tm p_now{};
		if (localtime_s(&p_now, &now) != 0) { throw std::runtime_error("Failed to get current date time."); }
		return date_time(p_now);
	}
}
