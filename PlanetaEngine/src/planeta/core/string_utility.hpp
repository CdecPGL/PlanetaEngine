#pragma once

#include <sstream>
#include <string>

namespace plnt::util {
	inline void convert_and_connect_to_string2(std::ostringstream &) {}

	template <typename First, typename... Rest>
	// NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
	void convert_and_connect_to_string2(std::ostringstream &oss, First &&first, Rest &&... rest) {
		oss << std::forward<First>(first);
		convert_and_connect_to_string2(oss, std::forward<Rest>(rest)...);
	}

	/*引数を文字列に変換して連結*/
	template <typename... Elements>
	// NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
	std::string convert_and_connect_to_string(Elements &&... elements) {
		std::ostringstream oss;
		convert_and_connect_to_string2(oss, std::forward<Elements>(elements)...);
		return std::move(oss.str());
	}

	/*文字が大文字アルファベッットか*/
	bool is_upper_case_character(char c);
	/*文字が小文字アルファベットか*/
	bool is_lower_case_character(char c);
}
