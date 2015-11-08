#pragma once

#include <stdexcept>

namespace planeta_engine {
	namespace core {
		class GameDataError final: public std::domain_error{
		public:
			GameDataError(const std::string& e) :std::domain_error(e) {}
		};
	}
}