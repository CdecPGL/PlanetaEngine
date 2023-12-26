#pragma once

#include <stdexcept>

namespace plnt {
	namespace util {
		class BadAlloc : public std::runtime_error {
		public:
			BadAlloc(const char *err) : runtime_error(err) { }
		};
	}
}
