#pragma once

#include <memory>
#include "File.hpp"
#include "DataContainer.hpp"

namespace plnt {
	namespace util {
		/*GameDataをシリアライズする*/
		std::shared_ptr<File> SerializeDataContainer(DataContainer &data_container);
		/*GameDataをデシリアライズする(失敗したらstd::runtime_errorを投げる)*/
		DataContainer DeserializeDataContainer(const std::shared_ptr<const File> &file);
	}
}
