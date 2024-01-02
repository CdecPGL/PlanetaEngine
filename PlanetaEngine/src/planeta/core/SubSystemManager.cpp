#include <functional>

#include "SubSystemManager.hpp"
#include "game.hpp"
#include "i_debug_manager.hpp"

namespace plnt::private_ {
	void sub_system_manager::create_debug_information_channel(const std::string &channel_id) {
		game::instance().debug_manager()->create_debug_information_channel(
			channel_id, [this]<typename T>(T &&ph1) { debug_information_add_handler(std::forward<T>(ph1)); });
	}

	void sub_system_manager::delete_debug_information_channel() {
		if (!debug_info_channel_id_.empty()) {
			game::instance().debug_manager()->delete_debug_information_channel(debug_info_channel_id_);
			debug_info_channel_id_.clear();
		}
	}
}
