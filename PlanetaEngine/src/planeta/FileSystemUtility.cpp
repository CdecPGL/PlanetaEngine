#include <fstream>
#include <algorithm>
#include "boost/algorithm/string.hpp"
#include "FileSystemUtility.hpp"
#include "File.hpp"
#include "planeta/LogUtility.hpp"

namespace plnt {
	namespace util {
		std::shared_ptr<File> CreateFileFromStandardFileInput(const std::string& file_name) {
			//ファイルを開く
			std::ifstream ifs(file_name, std::ios::binary);
			if (!ifs) {
				PE_LOG_ERROR("標準ファイル入力からファイル\"", file_name, "\"を開くことができませんでした。");
				return nullptr;
			}
			size_t size = (size_t)ifs.seekg(0, std::ios::end).tellg();
			ifs.clear();
			ifs.seekg(0, std::ios::beg);
			std::shared_ptr<File> file = std::make_shared<File>();
			if (!file->Reserve(size)) {
				PE_LOG_ERROR("標準ファイル入力からのファイル\"", file_name, "\"の領域", size, "バイトを確保できませんでした。");
				return nullptr;
			}
			ifs.read(reinterpret_cast<char*>(file->top_pointer()), size);
			if (ifs.bad()) {
				PE_LOG_ERROR("標準ファイル入力からファイル\"", file_name, "\"を読込中にエラーが発生しました。");
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