#pragma once

#include <stdexcept>

namespace plnt::util {
	class data_container_error final : public std::domain_error {
	public:
		explicit data_container_error(const std::string &e) : std::domain_error(e) { }
	};
}
