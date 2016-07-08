#pragma once

#include <string>

namespace planeta {
	namespace util {
		//UTF-8��Shift-JIS�ɕϊ�(WindowsAPI�Ɉˑ�)
		std::string ConvertUTF8ToShiftJIS(const std::string& str_utf8);
		//Shift-JIS��UTF-8�ɕϊ�(WindowsAPI�Ɉˑ�)
		std::string ConvertShiftJISToUTF8(const std::string& str_sjis);
	}
}