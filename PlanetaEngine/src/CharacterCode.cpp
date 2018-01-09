﻿#include "CharacterCode.hpp"

#include <memory>
#include <windows.h>

//http://sayahamitt.net/utf8%E3%81%AAstring%E5%85%A5%E3%82%8C%E3%81%9F%E3%82%89shiftjis%E3%81%AAstring%E5%87%BA%E3%81%A6%E3%81%8F%E3%82%8B%E9%96%A2%E6%95%B0%E4%BD%9C%E3%81%A3%E3%81%9F/ を参考にした。

namespace planeta {
	namespace util {

		std::string ConvertUTF8ToShiftJIS(const std::string& str_utf8) {
			return ConvertUTF16ToShiftJIS(ConvertUTF8ToUTF16(str_utf8));
		}

		std::string ConvertShiftJISToUTF8(const std::string& str_sjis) {
			return ConvertUTF16ToUTF8(ConvertShiftJISToUTF16(str_sjis));
		}

		std::wstring ConvertUTF8ToUTF16(const std::string& str_utf8) {
			//Unicodeへ変換後の文字列長を得る
			int length_unicode = MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), str_utf8.size() + 1, NULL, 0);
			//必要な分だけUnicode文字列のバッファを確保
			std::unique_ptr<wchar_t[]> buf_unicode = std::make_unique<wchar_t[]>(length_unicode);
			//UTF8からUnicodeへ変換
			MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), str_utf8.size() + 1, buf_unicode.get(), length_unicode);
			
			std::wstring str_unicode{ buf_unicode.get() };
			return str_unicode;
		}

		std::string ConvertUTF16ToUTF8(const std::wstring& str_utf16) {
			//UTF8へ変換後の文字列長を得る
			int length_utf8 = WideCharToMultiByte(CP_UTF8, 0, str_utf16.c_str(), -1, NULL, 0, NULL, NULL);
			//必要な分だけUTF8文字列のバッファを確保
			std::unique_ptr<char[]> buf_utf8 = std::make_unique<char[]>(length_utf8);
			//UnicodeからUTF8へ変換
			WideCharToMultiByte(CP_UTF8, 0, str_utf16.c_str(), str_utf16.size() + 1, buf_utf8.get(), length_utf8, NULL, NULL);

			std::string str_utf8(buf_utf8.get());
			return str_utf8;
		}

		std::wstring ConvertShiftJISToUTF16(const std::string& str_sjis) {
			//Unicodeへ変換後の文字列長を得る
			int length_unicode = MultiByteToWideChar(CP_THREAD_ACP, 0, str_sjis.c_str(), str_sjis.size() + 1, NULL, 0);
			//必要な分だけUnicode文字列のバッファを確保
			std::unique_ptr<wchar_t[]> buf_unicode = std::make_unique<wchar_t[]>(length_unicode);
			//ShiftJISからUnicodeへ変換
			MultiByteToWideChar(CP_THREAD_ACP, 0, str_sjis.c_str(), str_sjis.size() + 1, buf_unicode.get(), length_unicode);

			std::wstring str_unicode{ buf_unicode.get() };
			return str_unicode;
		}

		std::string ConvertUTF16ToShiftJIS(const std::wstring& str_utf16) {
			//SJISへ変換後の文字列長を得る
			int length_sjis = WideCharToMultiByte(CP_THREAD_ACP, 0, str_utf16.c_str(), -1, NULL, 0, NULL, NULL);
			//必要な分だけSJIS文字列のバッファを確保
			std::unique_ptr<char[]> buf_sjis = std::make_unique<char[]>(length_sjis);
			//UnicodeからSJISへ変換
			WideCharToMultiByte(CP_THREAD_ACP, 0, str_utf16.c_str(), str_utf16.size() + 1, buf_sjis.get(), length_sjis, NULL, NULL);

			std::string str_sjis(buf_sjis.get());
			return str_sjis;
		}

		std::string ConvertUTF8ToSystemCode(const std::string& str_utf8) {
#ifdef _MSC_VER
			return ConvertUTF8ToShiftJIS(str_utf8);
#endif
		}

		std::string ConvertSystemCodeToUTF8(const std::string& str_sys) {
#ifdef _MSC_VER
			return ConvertShiftJISToUTF8(str_sys);
#endif
		}

		std::string ConvertUTF16ToSystemCode(const std::wstring& str_utf16) {
#ifdef _MSC_VER
				return ConvertUTF16ToShiftJIS(str_utf16);
#endif
		}

		std::wstring ConvertSystemCodeToUTF16(const std::string& str_sys) {
#ifdef _MSC_VER
			return ConvertShiftJISToUTF16(str_sys);
#endif
		}

	}
}