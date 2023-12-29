#pragma once

#include <unordered_map>

#include "boost/optional.hpp"

namespace plnt {
	class file;

	/*! INIファイルを扱うためのクラス*/
	class ini_file final {
	public:
		using section_type = std::unordered_map<std::string, std::string>;
		using ini_type = std::unordered_map<std::string, section_type>;
		[[nodiscard]] ini_type::const_iterator begin() const { return data_.begin(); }
		[[nodiscard]] ini_type::const_iterator end() const { return data_.end(); }
		[[nodiscard]] ini_type::const_iterator find(const std::string &s) const { return data_.find(s); }
		[[nodiscard]] boost::optional<const section_type &> get_section(const std::string &s) const;
		bool load(const file &file);

	private:
		ini_type data_;
	};
}
