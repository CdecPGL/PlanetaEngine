#pragma once

#include <memory>
#include "File.h"
#include "GameDataElement.h"

namespace planeta_engine {
	namespace core {
		/*GameDataElement���V���A���C�Y����*/
		std::shared_ptr<file_system::File> SerializeGameDataElement(GameDataElement& game_data_element);
		/*GameDataElemet���f�V���A���C�Y����*/
		GameDataElement DeserializeGameDataElement(const std::shared_ptr<file_system::File>& file);
	}
}
