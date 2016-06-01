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
		/*�����𕶎���ɕϊ����ĘA��*/
		template<typename ... Elements>
		std::string ConvertAndConnectToString(Elements&&... elements) {
			std::ostringstream oss;
			ConvertAndConnectToString2(oss, std::forward<Elements>(elements)...);
			return std::move(oss.str());
		}
	}
}