#include "RJson.hpp"

namespace plnt {
	bool RJson::OnLoaded(const file &file, const json_file &metadata, ResourceReferencer &referencer) {
		json_file_ = std::make_unique<json_file>();
		if (!json_file_->load(file)) {
			PE_LOG_ERROR("JSONファイルの読み込みに失敗しました。");
			json_file_.release();
			return false;
		}
		return true;
	}

	void RJson::OnDisposed() { json_file_.release(); }

	const plnt::json_file &RJson::get_json_file() const { return *json_file_; }

	plnt::json_file &RJson::get_json_file() { return *json_file_; }
}
