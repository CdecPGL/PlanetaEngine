#include <functional>

#include "SubSystemManager.hpp"
#include "game.hpp"
#include "i_debug_manager.hpp"

namespace plnt {
	namespace private_ {
		void SubSystemManager::CreateDebugInformationChannel(const std::string &channel_id) {
			game::instance().debug_manager()->create_debug_information_channel(
				channel_id, std::bind(&SubSystemManager::DebugInfotmationAddHandler, this, std::placeholders::_1));
		}

		void SubSystemManager::DeleteDebugInformationChannel() {
			if (!debug_info_channel_id_.empty()) {
				game::instance().debug_manager()->delete_debug_information_channel(debug_info_channel_id_);
				debug_info_channel_id_.clear();
			}
		}
	}
}
