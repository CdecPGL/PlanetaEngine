#include <algorithm>

#include "../core/file_istream.hpp"

#include "r_ini.hpp"

namespace plnt {
	const ini_file &r_ini::get_ini_file() const { return *ini_file_; }

	ini_file &r_ini::get_ini_file() { return *ini_file_; }

	bool r_ini::on_loaded(const file &file, [[maybe_unused]] const json_file &metadata,
	                      [[maybe_unused]] resource_referencer &referencer) {
		ini_file_ = std::make_unique<ini_file>();
		if (!ini_file_->load(file)) {
			PE_LOG_ERROR("INIファイルの読み込みに失敗しました。");
			ini_file_.reset();
			return false;
		}
		return true;
	}

	void r_ini::on_disposed() { ini_file_.reset(); }
}
