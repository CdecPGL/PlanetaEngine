#pragma once

#include <unordered_map>
#include <string>
#include "boost/any.hpp"

namespace planeta_engine {
	class Event {
	public:
		
	private:
		int32_t event_type_id_;
		std::unordered_map<std::string, boost::any> arguments_;
	};
}