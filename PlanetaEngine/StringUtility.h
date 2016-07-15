#pragma once

#include <sstream>
#include <string>

namespace planeta {
	namespace util {
		inline void ConvertAndConnectToString2(std::ostringstream&) {}
		template<typename First, typename ... Rest>
		void ConvertAndConnectToString2(std::ostringstream& oss, First&& first, Rest&&... rest) {
			oss << std::forward<First>(first);
			ConvertAndConnectToString2(oss, std::forward<Rest>(rest)...);
		}
		/*引数を文字列に変換して連結*/
		template<typename ... Elements>
		std::string ConvertAndConnectToString(Elements&&... elements) {
			std::ostringstream oss;
			ConvertAndConnectToString2(oss, std::forward<Elements>(elements)...);
			return std::move(oss.str());
		}
		/*文字が大文字アルファベッットか*/
		bool IsUpperCaseCharacter(char c);
		/*文字が小文字アルファベットか*/
		bool IsLowerCaseCharacter(char c);
	}
}