#include <memory>
#include <windows.h>

#include "character_code.hpp"

//http://sayahamitt.net/utf8%E3%81%AAstring%E5%85%A5%E3%82%8C%E3%81%9F%E3%82%89shiftjis%E3%81%AAstring%E5%87%BA%E3%81%A6%E3%81%8F%E3%82%8B%E9%96%A2%E6%95%B0%E4%BD%9C%E3%81%A3%E3%81%9F/ を参考にした。

namespace plnt::util {
	std::string convert_utf8_to_shift_jis(const std::string &str_utf8) {
		return convert_utf16_to_shift_jis(convert_utf8_to_utf16(str_utf8));
	}

	std::string convert_shift_jis_to_utf8(const std::string &str_sjis) {
		return convert_utf16_to_utf8(convert_shift_jis_to_utf16(str_sjis));
	}

	std::wstring convert_utf8_to_utf16(const std::string &str_utf8) {
		//Unicodeへ変換後の文字列長を得る
		const int length_unicode = MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), str_utf8.size() + 1, nullptr, 0);
		//必要な分だけUnicode文字列のバッファを確保
		const auto buf_unicode = std::make_unique<wchar_t[]>(length_unicode);
		//UTF8からUnicodeへ変換
		MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), str_utf8.size() + 1, buf_unicode.get(), length_unicode);

		std::wstring str_unicode{buf_unicode.get()};
		return str_unicode;
	}

	std::string convert_utf16_to_utf8(const std::wstring &str_utf16) {
		//UTF8へ変換後の文字列長を得る
		const int length_utf8 = WideCharToMultiByte(CP_UTF8, 0, str_utf16.c_str(), -1, nullptr, 0, nullptr, nullptr);
		//必要な分だけUTF8文字列のバッファを確保
		const auto buf_utf8 = std::make_unique<char[]>(length_utf8);
		//UnicodeからUTF8へ変換
		WideCharToMultiByte(CP_UTF8, 0, str_utf16.c_str(), str_utf16.size() + 1, buf_utf8.get(), length_utf8, nullptr,
		nullptr);

		std::string str_utf8(buf_utf8.get());
		return str_utf8;
	}

	std::wstring convert_shift_jis_to_utf16(const std::string &str_sjis) {
		//Unicodeへ変換後の文字列長を得る
		const int length_unicode = MultiByteToWideChar(CP_THREAD_ACP, 0, str_sjis.c_str(), str_sjis.size() + 1, nullptr, 0);
		//必要な分だけUnicode文字列のバッファを確保
		const auto buf_unicode = std::make_unique<wchar_t[]>(length_unicode);
		//ShiftJISからUnicodeへ変換
		MultiByteToWideChar(CP_THREAD_ACP, 0, str_sjis.c_str(), str_sjis.size() + 1, buf_unicode.get(),
		                    length_unicode);

		std::wstring str_unicode{buf_unicode.get()};
		return str_unicode;
	}

	std::string convert_utf16_to_shift_jis(const std::wstring &str_utf16) {
		//SJISへ変換後の文字列長を得る
		const int length_sjis = WideCharToMultiByte(CP_THREAD_ACP, 0, str_utf16.c_str(), -1, nullptr, 0, nullptr, nullptr);
		//必要な分だけSJIS文字列のバッファを確保
		const auto buf_sjis = std::make_unique<char[]>(length_sjis);
		//UnicodeからSJISへ変換
		WideCharToMultiByte(CP_THREAD_ACP, 0, str_utf16.c_str(), str_utf16.size() + 1, buf_sjis.get(), length_sjis,
		                    nullptr, nullptr);

		std::string str_sjis(buf_sjis.get());
		return str_sjis;
	}

	std::string convert_utf8_to_system_code(const std::string &str_utf8) {
		#ifdef _MSC_VER
		return convert_utf8_to_shift_jis(str_utf8);
		#endif
	}

	std::string convert_system_code_to_utf8(const std::string &str_sys) {
		#ifdef _MSC_VER
		return convert_shift_jis_to_utf8(str_sys);
		#endif
	}

	std::string convert_utf16_to_system_code(const std::wstring &str_utf16) {
		#ifdef _MSC_VER
		return convert_utf16_to_shift_jis(str_utf16);
		#endif
	}

	std::wstring convert_system_code_to_utf16(const std::string &str_sys) {
		#ifdef _MSC_VER
		return convert_shift_jis_to_utf16(str_sys);
		#endif
	}
}
