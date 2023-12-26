//boost::splitに関するC4996エラーをこのファイルのみ無効化
#pragma warning(push)
#pragma warning(disable:4996)

#include "CsvFile.hpp"

#include <algorithm>
#include "boost/algorithm/string.hpp"
#include "File.hpp"
#include "LogUtility.hpp"
#include "CharacterCode.hpp"


namespace plnt {
	bool CsvFile::Load(const File &file) {
		size_t size = file.size();
		const char *c_ptr = (const char *)file.top_pointer();
		unsigned int pos = 0;
		std::vector<std::string> lines;
		std::string buf;
		while (pos < size) {
			char w = c_ptr[pos];
			if (w == '\r') { } else if (w == '\n') {
				lines.push_back(move(buf));
				buf.clear();
			} else { buf.push_back(w); }
			++pos;
		}
		//	buf.pop_back();
		if (buf.size() != 0) { lines.push_back(move(buf)); }
		return _CreateCSVFromLines(lines);
	}

	bool CsvFile::_CreateCSVFromLines(std::vector<std::string> &lines) {
		loaded_datas.clear();
		using namespace boost;
		//無効な行を取り除く(空だったら)
		std::erase_if(lines, [](std::string line)-> bool {
			if (line.size() == 0)return true;
			return false;
		});
		//行がなかったら警告
		if (lines.size() == 0) {
			PE_LOG_WARNING("空のSCVファイルです。又は行の抽出に失敗しました。");
			return true;
		}
		//データ抽出
		int i = 1;
		for (std::string &line : lines) {
			std::vector<std::string> datas;
			algorithm::split(datas, line, algorithm::is_any_of(","));
			if (datas.size() == 0) {
				PE_LOG_ERROR("csvファイルの読み込みエラー。:データの抽出に失敗しました(", i, "行目)。");
				return false;
			}
			++i;
			//\nを取り除く
			for (std::string &data : datas) { std::erase_if(data, [](char w)-> bool { return w == '/n'; }); }
			//データを保存
			loaded_datas.push_back(std::move(datas));
		}

		//読み込んだデータの文字コードをUTF8からシステム文字コードへ変換
		for (auto &&line : loaded_datas) { for (auto &&elem : line) { elem = util::ConvertUTF8ToSystemCode(elem); } }
		return true;
	}

	CsvFile::CSV_DATA::iterator CsvFile::begin() { return loaded_datas.begin(); }
	CsvFile::CSV_DATA::const_iterator CsvFile::cbegin() const { return loaded_datas.cbegin(); }

	CsvFile::CSV_DATA::iterator CsvFile::end() { return loaded_datas.end(); }
	CsvFile::CSV_DATA::const_iterator CsvFile::cend() const { return loaded_datas.cend(); }
}

#pragma warning(pop)
