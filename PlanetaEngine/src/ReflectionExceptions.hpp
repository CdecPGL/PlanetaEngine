#pragma once

#include <stdexcept>

namespace plnt {
	class reflection_error : public std::runtime_error{
		using runtime_error::runtime_error;
	};
}