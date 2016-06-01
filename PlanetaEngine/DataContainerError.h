#pragma once

#include <stdexcept>

namespace planeta_engine {
	namespace util {
		class DataContainerError final: public std::domain_error{
		public:
			DataContainerError(const std::string& e) :std::domain_error(e) {}
		};
	}
}