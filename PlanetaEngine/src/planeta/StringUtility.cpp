#include <ctype.h>

#include "planeta/StringUtility.hpp"

namespace plnt {
	namespace util {

		bool IsUpperCaseCharacter(char c) {
			return isupper(static_cast<unsigned char> (c)) > 0;
		}

		bool IsLowerCaseCharacter(char c) {
			return islower(static_cast<unsigned char> (c)) > 0;
		}

	}
}