#include "RCsv.hpp"

namespace plnt {
	const plnt::CsvFile &RCsv::csv_file() const { return *csv_file_; }

	plnt::CsvFile &RCsv::csv_file() { return *csv_file_; }

	bool RCsv::OnLoaded(const File &file, const JsonFile &metadata, ResourceReferencer &referencer) {
		csv_file_ = std::make_unique<CsvFile>();
		if (!csv_file_->Load(file)) {
			PE_LOG_ERROR("CSVファイルの読み込みに失敗しました。");
			csv_file_.release();
			return false;
		}
		return true;
	}

	void RCsv::OnDisposed() { csv_file_.release(); }
}
