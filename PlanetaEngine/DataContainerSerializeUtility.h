#pragma once

#include <memory>
#include "File.h"
#include "DataContainer.h"

namespace planeta_engine {
	namespace utility {
		/*GameData���V���A���C�Y����*/
		std::shared_ptr<File> SerializeDataContainer(DataContainer& data_container);
		/*GameData���f�V���A���C�Y����(���s������std::runtime_error�𓊂���)*/
		DataContainer DeserializeDataContainer(const std::shared_ptr<const File>& file);
	}
}
