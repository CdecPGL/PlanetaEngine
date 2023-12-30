#include "RJson.hpp"

namespace plnt {
	bool RJson::on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) {
		json_file_ = std::make_unique<json_file>();
		if (!json_file_->load(file)) {
			PE_LOG_ERROR("JSONファイルの読み込みに失敗しました。");
			json_file_.release();
			return false;
		}
		return true;
	}

	void RJson::on_disposed() { json_file_.release(); }

	const plnt::json_file &RJson::get_json_file() const { return *json_file_; }

	plnt::json_file &RJson::get_json_file() { return *json_file_; }
}
