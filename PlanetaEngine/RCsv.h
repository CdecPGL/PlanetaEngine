#pragma once

#include "ResourceBase.h"
#include "CsvFile.h"

namespace planeta {
	class RCsv : public ResourceBase {
	public:
		const CsvFile& csv_file()const;
		CsvFile& csv_file();
	private:
		std::unique_ptr<CsvFile> csv_file_;

		bool _Create(const File& file, ResourceReferencer&) override;
		void _Dispose()override;
	};
}
