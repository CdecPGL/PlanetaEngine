#include "RCsv.h"

namespace planeta {

	const planeta::CsvFile& RCsv::csv_file() const {
		return *csv_file_;
	}

	planeta::CsvFile& RCsv::csv_file() {
		return *csv_file_;
	}

	bool RCsv::_Create(const File& file, ResourceReferencer&) {
		csv_file_ = std::make_unique<CsvFile>();
		if (!csv_file_->Load(file)) {
			PE_LOG_ERROR("CSVファイルの読み込みに失敗しました。");
			csv_file_.release();
			return false;
		}
		return true;
	}

	void RCsv::_Dispose() {
		csv_file_.release();
	}

}