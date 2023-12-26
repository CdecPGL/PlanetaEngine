#pragma once

#include <string>

namespace plnt {
	namespace util {
		//UTF-8をShift-JISに変換(WindowsAPIに依存)
		std::string ConvertUTF8ToShiftJIS(const std::string &str_utf8);
		//Shift-JISをUTF-8に変換(WindowsAPIに依存)
		std::string ConvertShiftJISToUTF8(const std::string &str_sjis);
		//UTF-8をUTF-16に変換(WindowsAPIに依存)
		std::wstring ConvertUTF8ToUTF16(const std::string &str_utf8);
		//UTF-16をUTF-8に変換(WindowsAPIに依存)
		std::string ConvertUTF16ToUTF8(const std::wstring &str_utf16);
		//Shift-JISをUTF-16に変換(WindowsAPIに依存)
		std::wstring ConvertShiftJISToUTF16(const std::string &str_sjis);
		//UTF-16をShift-JISに変換(WindowsAPIに依存)
		std::string ConvertUTF16ToShiftJIS(const std::wstring &str_utf16);
		//UTF-8をシステムコードに変換(WindowsAPIに依存)
		std::string ConvertUTF8ToSystemCode(const std::string &str_utf8);
		//システムコードをUTF-8に変換(WindowsAPIに依存)
		std::string ConvertSystemCodeToUTF8(const std::string &str_sys);
		//UTF-16をシステムコードに変換(WindowsAPIに依存)
		std::string ConvertUTF16ToSystemCode(const std::wstring &str_utf16);
		//システムコードをUTF-16に変換(WindowsAPIに依存)
		std::wstring ConvertSystemCodeToUTF16(const std::string &str_sys);
	}
}
