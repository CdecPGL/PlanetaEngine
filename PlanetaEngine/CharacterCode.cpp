#include "CharacterCode.h"

#include <memory>
#include <windows.h>

//http://sayahamitt.net/utf8%E3%81%AAstring%E5%85%A5%E3%82%8C%E3%81%9F%E3%82%89shiftjis%E3%81%AAstring%E5%87%BA%E3%81%A6%E3%81%8F%E3%82%8B%E9%96%A2%E6%95%B0%E4%BD%9C%E3%81%A3%E3%81%9F/ ���Q�l�ɂ����B

namespace planeta {
	namespace util {

		std::string ConvertUTF8ToShiftJIS(const std::string& str_utf8) {
			//Unicode�֕ϊ���̕����񒷂𓾂�
			int length_unicode = MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), str_utf8.size() + 1, NULL, 0);

			//�K�v�ȕ�����Unicode������̃o�b�t�@���m��
			std::unique_ptr<wchar_t[]> buf_unicode = std::make_unique<wchar_t[]>(length_unicode);

			//UTF8����Unicode�֕ϊ�
			MultiByteToWideChar(CP_UTF8, 0, str_utf8.c_str(), str_utf8.size() + 1, buf_unicode.get(), length_unicode);

			//ShiftJIS�֕ϊ���̕����񒷂𓾂�
			int length_sjis = WideCharToMultiByte(CP_THREAD_ACP, 0, buf_unicode.get(), -1, NULL, 0, NULL, NULL);

			//�K�v�ȕ�����ShiftJIS������̃o�b�t�@���m��
			std::unique_ptr<char[]> buf_sjis = std::make_unique<char[]>(length_sjis);

			//Unicode����ShiftJIS�֕ϊ�
			WideCharToMultiByte(CP_THREAD_ACP, 0, buf_unicode.get(), length_unicode + 1, buf_sjis.get(), length_sjis, NULL, NULL);

			std::string str_sjis(buf_sjis.get());

			return str_sjis;
		}

		std::string ConvertShiftJISToUTF8(const std::string& str_sjis) {
			//Unicode�֕ϊ���̕����񒷂𓾂�
			int length_unicode = MultiByteToWideChar(CP_THREAD_ACP, 0, str_sjis.c_str(), str_sjis.size() + 1, NULL, 0);

			//�K�v�ȕ�����Unicode������̃o�b�t�@���m��
			std::unique_ptr<wchar_t[]> buf_unicode = std::make_unique<wchar_t[]>(length_unicode);

			//ShiftJIS����Unicode�֕ϊ�
			MultiByteToWideChar(CP_THREAD_ACP, 0, str_sjis.c_str(), str_sjis.size() + 1, buf_unicode.get(), length_unicode);


			//UTF8�֕ϊ���̕����񒷂𓾂�
			int length_utf8 = WideCharToMultiByte(CP_UTF8, 0, buf_unicode.get(), -1, NULL, 0, NULL, NULL);

			//�K�v�ȕ�����UTF8������̃o�b�t�@���m��
			std::unique_ptr<char[]> buf_utf8 = std::make_unique<char[]>(length_utf8);

			//Unicode����UTF8�֕ϊ�
			WideCharToMultiByte(CP_UTF8, 0, buf_unicode.get(), length_unicode + 1, buf_utf8.get(), length_utf8, NULL, NULL);

			std::string str_utf8(buf_utf8.get());

			return str_utf8;
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

	}
}