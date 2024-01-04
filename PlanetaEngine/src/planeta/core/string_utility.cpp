#include <cctype>
#include <locale>

#include "string_utility.hpp"

namespace plnt::util {
	bool is_upper_case_character(const char c) { return isupper(static_cast<unsigned char>(c)) > 0; }
	bool is_lower_case_character(const char c) { return islower(static_cast<unsigned char>(c)) > 0; }
	char to_lower(const char c) { return std::tolower(c, std::locale::classic()); }
	char to_upper(const char c) { return std::toupper(c, std::locale::classic()); }
}
