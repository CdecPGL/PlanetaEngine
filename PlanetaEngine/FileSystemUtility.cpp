#include <fstream>
#include <algorithm>
#include "boost/algorithm/string.hpp"
#include "FileSystemUtility.h"
#include "File.h"
#include "SystemLog.h"

namespace planeta {
	namespace util {
		std::shared_ptr<File> CreateFileFromStandardFileInput(const std::string& file_name) {
			//�t�@�C�����J��
			std::ifstream ifs(file_name, std::ios::binary);
			if (!ifs) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, "�W���t�@�C�����͂���t�@�C��\"", file_name, "\"���J�����Ƃ��ł��܂���ł����B", __FUNCTION__);
				return nullptr;
			}
			size_t size = (size_t)ifs.seekg(0, std::ios::end).tellg();
			ifs.clear();
			ifs.seekg(0, std::ios::beg);
			std::shared_ptr<File> file = std::make_shared<File>();
			if (!file->ChangeSize(size)) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, "�W���t�@�C�����͂���̃t�@�C��\"", file_name, "\"�̗̈�", size, "�o�C�g���m�ۂł��܂���ł����B", __FUNCTION__);
				return nullptr;
			}
			ifs.read(reinterpret_cast<char*>(file->GetTopPointer()), size);
			if (ifs.bad()) {
				debug::SystemLog::instance().Log(debug::LogLevel::Error, "�W���t�@�C�����͂���t�@�C��\"", file_name, "\"��Ǎ����ɃG���[���������܂����B", __FUNCTION__);
				return nullptr;
			}
			return file;
		}

		void UnifyLineFeedCode(std::string& str) {
			boost::algorithm::replace_all(str, "\r\n", "\n");
			boost::algorithm::replace_all(str, "\r", "\n");
		}

		void RemoveLineFeedCode(std::string& str) {
			str.erase(std::remove_if(str.begin(), str.end(), [](char c) {return c == '\r' || c == '\n'; }), str.end());
		}

		void RemoveSpaceAndTab(std::string& str) {
			str.erase(std::remove_if(str.begin(), str.end(), [](char c) {return c == ' ' || c == '\t'; }), str.end());
		}

	}
}