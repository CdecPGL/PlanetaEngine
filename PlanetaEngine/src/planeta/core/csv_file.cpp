//boost::splitに関するC4996エラーをこのファイルのみ無効化
#pragma warning(push)
#pragma warning(disable:4996)

#include <algorithm>

#include "boost/algorithm/string.hpp"

#include "csv_file.hpp"
#include "file.hpp"
#include "log_utility.hpp"
#include "character_code.hpp"

namespace plnt {
	bool csv_file::load(const file &file) {
		const size_t size = file.size();
		const auto c_ptr = reinterpret_cast<const char *>(file.top_pointer());
		unsigned int pos = 0;
		std::vector<std::string> lines;
		std::string buf;
		while (pos < size) {
			if (const char w = c_ptr[pos]; w == '\r') { } else if (w == '\n') {
				lines.push_back(std::move(buf));
				buf.clear();
			} else { buf.push_back(w); }
			++pos;
		}
		//	buf.pop_back();
		if (!buf.empty()) { lines.push_back(move(buf)); }
		return create_csv_from_lines(lines);
	}

	bool csv_file::create_csv_from_lines(std::vector<std::string> &lines) {
		loaded_datas_.clear();
		using namespace boost;
		//無効な行を取り除く(空だったら)
		std::erase_if(lines, [](const std::string &line)-> bool {
			if (line.empty())return true;
			return false;
		});
		//行がなかったら警告
		if (lines.empty()) {
			PE_LOG_WARNING("空のSCVファイルです。又は行の抽出に失敗しました。");
			return true;
		}
		//データ抽出
		int i = 1;
		for (std::string &line : lines) {
			std::vector<std::string> datas;
			split(datas, line, is_any_of(","));
			if (datas.empty()) {
				PE_LOG_ERROR("csvファイルの読み込みエラー。:データの抽出に失敗しました(", i, "行目)。");
				return false;
			}
			++i;
			//\nを取り除く
			for (std::string &data : datas) { std::erase_if(data, [](const char w)-> bool { return w == '\n'; }); }
			//データを保存
			loaded_datas_.push_back(std::move(datas));
		}

		//読み込んだデータの文字コードをUTF8からシステム文字コードへ変換
		for (auto &&line : loaded_datas_) {
			for (auto &&elem : line) { elem = util::convert_utf8_to_system_code(elem); }
		}
		return true;
	}

	csv_file::csv_data::iterator csv_file::begin() { return loaded_datas_.begin(); }
	csv_file::csv_data::const_iterator csv_file::cbegin() const { return loaded_datas_.cbegin(); }

	csv_file::csv_data::iterator csv_file::end() { return loaded_datas_.end(); }
	csv_file::csv_data::const_iterator csv_file::cend() const { return loaded_datas_.cend(); }
}

#pragma warning(pop)
