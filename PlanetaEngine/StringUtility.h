#pragma once

#include <sstream>
#include <string>

namespace planeta_engine {
	namespace utility {
		inline void ConvertAndConnectToString2(std::ostringstream&) {}
		template<typename First, typename ... Rest>
		void ConvertAndConnectToString2(std::ostringstream& oss, First first, Rest... rest) {
			oss << first;
			ConvertAndConnectToString2(oss, rest...);
		}
		/*ˆø”‚ð•¶Žš—ñ‚É•ÏŠ·‚µ‚Ä˜AŒ‹*/
		template<typename ... Elements>
		std::string ConvertAndConnectToString(Elements... elements) {
			std::ostringstream oss;
			ConvertAndConnectToString2(oss, elements...);
			return std::move(oss.str());
		}
	}
}