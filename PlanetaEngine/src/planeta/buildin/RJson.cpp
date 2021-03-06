﻿#include "RJson.hpp"

namespace plnt {
	bool RJson::OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) {
		json_file_ = std::make_unique<JsonFile>();
		if (!json_file_->Load(file)) {
			PE_LOG_ERROR("JSONファイルの読み込みに失敗しました。");
			json_file_.release();
			return false;
		}
		return true;
	}

	void RJson::OnDisposed() {
		json_file_.release();
	}

	const plnt::JsonFile& RJson::json_file() const {
		return *json_file_;
	}

	plnt::JsonFile& RJson::json_file() {
		return *json_file_;
	}

}
