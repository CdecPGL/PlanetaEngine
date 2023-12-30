#pragma once

#include <stdexcept>
#include <string>

namespace plnt {
	class null_weak_pointer_exception final : public std::runtime_error {
	public:
		explicit null_weak_pointer_exception(const std::string &str) : std::runtime_error(str) { }
	};
}
