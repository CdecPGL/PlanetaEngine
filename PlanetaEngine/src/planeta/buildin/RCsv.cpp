#include "RCsv.hpp"

namespace plnt {
	const csv_file &RCsv::csv_file() const { return *csv_file_; }

	csv_file &RCsv::csv_file() { return *csv_file_; }

	bool RCsv::on_loaded(const file &file, const json_file &metadata, resource_referencer &referencer) {
		csv_file_ = std::make_unique<plnt::csv_file>();
		if (!csv_file_->load(file)) {
			PE_LOG_ERROR("CSVファイルの読み込みに失敗しました。");
			csv_file_.release();
			return false;
		}
		return true;
	}

	void RCsv::on_disposed() { csv_file_.release(); }
}
