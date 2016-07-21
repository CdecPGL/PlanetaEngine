#pragma once
#include <stdexcept>

namespace planeta{
	namespace core {
		class BadAlloc : public std::runtime_error {
		public:
			BadAlloc(const char* err) :runtime_error(err) {}
		};
	}
}