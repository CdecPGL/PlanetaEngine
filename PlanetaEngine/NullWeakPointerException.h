#pragma once

#include <stdexcept>
#include <string>

namespace planeta_engine {
	namespace utility {
		class NullWeakPointerException : public std::runtime_error {
		public:
			NullWeakPointerException(const std::string& str) :std::runtime_error(str) {}
		};
	}
}
