#pragma once

#include <string>
#include <vector>

namespace plnt {
	class File;

	/*! Csvファイルクラス*/
	class CsvFile final {
	public:
		using CSV_DATA = std::vector<std::vector<std::string>>;
		CSV_DATA::iterator begin();
		CSV_DATA::const_iterator cbegin() const;
		CSV_DATA::iterator end();
		CSV_DATA::const_iterator cend() const;
		bool Load(const File &file);

	private:
		std::vector<std::vector<std::string>> loaded_datas;
		bool _CreateCSVFromLines(std::vector<std::string> &);
	};
}
