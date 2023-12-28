#pragma once

#include <string>
#include <vector>

namespace plnt {
	class file;

	/*! Csvファイルクラス*/
	class csv_file final {
	public:
		using csv_data = std::vector<std::vector<std::string>>;
		csv_data::iterator begin();
		[[nodiscard]] csv_data::const_iterator cbegin() const;
		csv_data::iterator end();
		[[nodiscard]] csv_data::const_iterator cend() const;
		bool load(const file &file);

	private:
		std::vector<std::vector<std::string>> loaded_datas_;
		bool create_csv_from_lines(std::vector<std::string> &);
	};
}
