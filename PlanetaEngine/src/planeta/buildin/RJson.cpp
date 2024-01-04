﻿#include "RJson.hpp"

namespace plnt {
	bool r_json::on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) {
		json_file_ = std::make_unique<json_file>();
		if (!json_file_->load(file)) {
			PE_LOG_ERROR("JSONファイルの読み込みに失敗しました。");
			json_file_.reset();
			return false;
		}
		return true;
	}

	void r_json::on_disposed() { json_file_.reset(); }

	const json_file &r_json::get_json_file() const { return *json_file_; }

	json_file &r_json::get_json_file() { return *json_file_; }
}
