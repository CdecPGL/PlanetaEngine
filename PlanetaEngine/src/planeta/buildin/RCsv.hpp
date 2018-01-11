#pragma once

#include "planeta/core/ResourceBase.hpp"
#include "planeta/core/CsvFile.hpp"

namespace plnt {
	class RCsv : public ResourceBase {
	public:
		const CsvFile& csv_file()const;
		CsvFile& csv_file();
	private:
		std::unique_ptr<CsvFile> csv_file_;

		bool OnLoaded(const File& file, const JsonFile& metadata, ResourceReferencer& referencer) override;
		void OnDisposed()override;
	};
}
