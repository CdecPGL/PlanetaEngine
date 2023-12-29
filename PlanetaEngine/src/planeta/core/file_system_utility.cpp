#include <fstream>

#include "boost/algorithm/string.hpp"

#include "file_system_utility.hpp"
#include "file.hpp"
#include "log_utility.hpp"

namespace plnt::util {
	std::shared_ptr<file> create_file_from_standard_file_input(const std::string &file_name) {
		//ファイルを開く
		std::ifstream ifs(file_name, std::ios::binary);
		if (!ifs) {
			PE_LOG_ERROR("標準ファイル入力からファイル\"", file_name, "\"を開くことができませんでした。");
			return nullptr;
		}
		size_t size = ifs.seekg(0, std::ios::end).tellg();
		ifs.clear();
		ifs.seekg(0, std::ios::beg);
		auto file = std::make_shared<plnt::file>();
		if (!file->reserve(size)) {
			PE_LOG_ERROR("標準ファイル入力からのファイル\"", file_name, "\"の領域", size, "バイトを確保できませんでした。");
			return nullptr;
		}
		if (!std::in_range<std::streamsize>(size)) {
			PE_LOG_ERROR("標準ファイル入力からファイル\"", file_name, "\"を読込中にエラーが発生しました。入力サイズが最大サイズを超えています。");
			return nullptr;
		}
		ifs.read(reinterpret_cast<char *>(file->top_pointer()), static_cast<std::streamsize>(size));
		if (ifs.bad()) {
			PE_LOG_ERROR("標準ファイル入力からファイル\"", file_name, "\"を読込中にエラーが発生しました。");
			return nullptr;
		}
		return file;
	}

	void unify_line_feed_code(std::string &str) {
		boost::algorithm::replace_all(str, "\r\n", "\n");
		boost::algorithm::replace_all(str, "\r", "\n");
	}

	void remove_line_feed_code(std::string &str) {
		std::erase_if(str, [](const char c) { return c == '\r' || c == '\n'; });
	}

	void remove_space_and_tab(std::string &str) {
		std::erase_if(str, [](const char c) { return c == ' ' || c == '\t'; });
	}
}
