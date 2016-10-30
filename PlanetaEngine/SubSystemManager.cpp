#include <functional>

#include "SubSystemManager.h"
#include "Game.h"
#include "IDebugManager.h"

namespace planeta {
	namespace private_{
		void SubSystemManager::CreateDebugInformationChannel(const std::string& channel_id) {
			Game::instance().debug_manager()->CreateDebugInformationChannel(channel_id, std::bind(&SubSystemManager::DebugInfotmationAddHandler, this, std::placeholders::_1));
		}

		void SubSystemManager::DeleteDebugInformationChannel() {
			if (!debug_info_channel_id_.empty()) {
				Game::instance().debug_manager()->DeleteDebugInformationChannel(debug_info_channel_id_);
				debug_info_channel_id_.clear();
			}
		}

	}
}