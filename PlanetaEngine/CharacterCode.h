#pragma once

#include <string>

namespace planeta {
	namespace util {
		//UTF-8‚ðShift-JIS‚É•ÏŠ·(WindowsAPI‚ÉˆË‘¶)
		std::string ConvertUTF8ToShiftJIS(const std::string& str_utf8);
		//Shift-JIS‚ðUTF-8‚É•ÏŠ·(WindowsAPI‚ÉˆË‘¶)
		std::string ConvertShiftJISToUTF8(const std::string& str_sjis);
	}
}