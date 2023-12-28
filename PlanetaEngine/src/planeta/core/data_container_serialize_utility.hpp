#pragma once

#include <memory>

#include "File.hpp"
#include "data_container.hpp"

namespace plnt::util {
	/*GameDataをシリアライズする*/
	std::shared_ptr<File> serialize_data_container(data_container &data_container);
	/*GameDataをデシリアライズする(失敗したらstd::runtime_errorを投げる)*/
	data_container deserialize_data_container(const std::shared_ptr<const File> &file);
}
