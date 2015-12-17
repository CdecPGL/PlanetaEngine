#pragma once

#include <list>
#include <unordered_map>
#include <string>
#include "InsertPosIndication.h"

namespace planeta_engine {
	namespace core {
		class GameProcessKeyPositionDefinition{
		public:
			GameProcessKeyPositionDefinition();
			bool AddKeyPosition(const std::string& id, InsertPosIndication ins_ind, const std::string& pos_id);
			const std::list<std::string>& GetKeyPositionIDList()const { return key_position_id_list_; }
		private:
			std::list<std::string> key_position_id_list_;
			std::unordered_map<std::string, std::list<std::string>::iterator> id_it_map_;
		};
	}
}