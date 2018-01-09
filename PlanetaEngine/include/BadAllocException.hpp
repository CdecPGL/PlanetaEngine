#pragma once
#include <stdexcept>

namespace plnt{
	namespace private_ {
		class BadAlloc : public std::runtime_error {
		public:
			BadAlloc(const char* err) :runtime_error(err) {}
		};
	}
}