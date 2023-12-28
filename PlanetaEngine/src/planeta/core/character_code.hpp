#pragma once

#include <string>

namespace plnt::util {
	//UTF-8をShift-JISに変換(WindowsAPIに依存)
	std::string convert_utf8_to_shift_jis(const std::string &str_utf8);
	//Shift-JISをUTF-8に変換(WindowsAPIに依存)
	std::string convert_shift_jis_to_utf8(const std::string &str_sjis);
	//UTF-8をUTF-16に変換(WindowsAPIに依存)
	std::wstring convert_utf8_to_utf16(const std::string &str_utf8);
	//UTF-16をUTF-8に変換(WindowsAPIに依存)
	std::string convert_utf16_to_utf8(const std::wstring &str_utf16);
	//Shift-JISをUTF-16に変換(WindowsAPIに依存)
	std::wstring convert_shift_jis_to_utf16(const std::string &str_sjis);
	//UTF-16をShift-JISに変換(WindowsAPIに依存)
	std::string convert_utf16_to_shift_jis(const std::wstring &str_utf16);
	//UTF-8をシステムコードに変換(WindowsAPIに依存)
	std::string convert_utf8_to_system_code(const std::string &str_utf8);
	//システムコードをUTF-8に変換(WindowsAPIに依存)
	std::string convert_system_code_to_utf8(const std::string &str_sys);
	//UTF-16をシステムコードに変換(WindowsAPIに依存)
	std::string convert_utf16_to_system_code(const std::wstring &str_utf16);
	//システムコードをUTF-16に変換(WindowsAPIに依存)
	std::wstring convert_system_code_to_utf16(const std::string &str_sys);
}
