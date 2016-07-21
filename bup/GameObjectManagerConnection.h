#pragma once

namespace planeta {
	class GameObjectManager;
	class GameObjectManagerConnection final {
	public:
		GameObjectManagerConnection(GameObjectManager& gom, int id) :game_object_manager_(gom), id_(id) {}
		bool RequestActivation();
		bool RequestInactivation();
		void RequestDisposal();
	private:
		int id_;
		bool connection_valid_ = true;
		GameObjectManager& game_object_manager_;
	};
}
