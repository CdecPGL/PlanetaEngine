#include "game_object_manager_connection.hpp"
#include "game_object_manager.hpp"

namespace plnt::private_ {
	bool game_object_manager_connection::request_activation() const { return game_object_manager_.activate_game_object(id_); }

	bool game_object_manager_connection::request_inactivation() const {
		return game_object_manager_.inactivate_game_object(id_);
	}

	void game_object_manager_connection::request_disposal() {
		if (connection_valid_) {
			const auto ret = game_object_manager_.remove_game_object(id_);
			connection_valid_ = false;
			assert(ret == true);
		}
	}
}
