#pragma once

#include <string>

namespace planeta {
	namespace util {
		//UTF-8をShift-JISに変換(WindowsAPIに依存)
		std::string ConvertUTF8ToShiftJIS(const std::string& str_utf8);
		//Shift-JISをUTF-8に変換(WindowsAPIに依存)
		std::string ConvertShiftJISToUTF8(const std::string& str_sjis);
	}
}