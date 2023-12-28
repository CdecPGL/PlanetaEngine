#pragma once

namespace plnt::private_ {
	class game_object_manager;

	class game_object_manager_connection final {
	public:
		game_object_manager_connection(game_object_manager &gom, const int id) : id_(id), game_object_manager_(gom) { }

		[[nodiscard]] bool request_activation() const;
		[[nodiscard]] bool request_inactivation() const;
		void request_disposal();

	private:
		int id_;
		bool connection_valid_ = true;
		game_object_manager &game_object_manager_;
	};
}
