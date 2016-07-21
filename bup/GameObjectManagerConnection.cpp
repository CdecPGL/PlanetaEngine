#include "GameObjectManagerConnection.h"

#include "GameObjectManager.h"

namespace planeta {

	bool GameObjectManagerConnection::RequestActivation() {
		return game_object_manager_.ActivateGameObject(id_);
	}

	bool GameObjectManagerConnection::RequestInactivation() {
		return game_object_manager_.InActivateGameObject(id_);
	}

	void GameObjectManagerConnection::RequestDisposal() {
		if (connection_valid_) {
			auto ret = game_object_manager_.RemoveGameObject(id_);
			connection_valid_ = false;
			assert(ret == true);
		}
	}
}