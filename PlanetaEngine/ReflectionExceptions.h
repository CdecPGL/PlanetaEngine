#pragma once

#include <stdexcept>

namespace planeta {
	class reflection_error : public std::runtime_error{
		using runtime_error::runtime_error;
	};
}