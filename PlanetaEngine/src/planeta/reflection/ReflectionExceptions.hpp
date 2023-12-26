#pragma once

#include <stdexcept>

namespace plnt::reflection {
	class reflection_error : public std::runtime_error {
		using runtime_error::runtime_error;
	};
}
