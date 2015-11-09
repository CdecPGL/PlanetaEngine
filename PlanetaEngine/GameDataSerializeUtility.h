#pragma once

#include <memory>
#include "File.h"
#include "GameDataElement.h"

namespace planeta_engine {
	namespace core {
		/*GameDataElementをシリアライズする*/
		std::shared_ptr<file_system::File> SerializeGameDataElement(GameDataElement& game_data_element);
		/*GameDataElemetをデシリアライズする*/
		GameDataElement DeserializeGameDataElement(const std::shared_ptr<file_system::File>& file);
	}
}
