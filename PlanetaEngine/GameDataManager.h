#pragma once

#include <unordered_map>
#include <memory>
#include "GameDataElement.h"

namespace planeta_engine {
	namespace core {
		class GameDataManager {
		public:
			std::shared_ptr<GameDataElement> GetElement(const std::string& element_id);

		private:
			std::unordered_map<std::string, std::shared_ptr<GameDataElement>> elements_;
		};
	}
}