#pragma once

#include <memory>
#include "File.h"
#include "DataContainer.h"

namespace planeta_engine {
	namespace utility {
		/*GameDataをシリアライズする*/
		std::shared_ptr<file_system::File> SerializeDataContainer(DataContainer& data_container);
		/*GameDataをデシリアライズする(失敗したらstd::runtime_errorを投げる)*/
		DataContainer DeserializeDataContainer(const std::shared_ptr<const file_system::File>& file);
	}
}
