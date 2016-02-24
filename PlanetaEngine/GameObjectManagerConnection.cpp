#include "GameObjectManagerConnection.h"

#include "GameObjectManager.h"

namespace planeta_engine {
	namespace game {

		bool GameObjectManagerConnection::Activate()
		{
			return game_object_manager_.ActivateGameObject(id_);
		}

		bool GameObjectManagerConnection::InActivate()
		{
			return game_object_manager_.InActivateGameObject(id_);
		}

		void GameObjectManagerConnection::Dispose()
		{
			if (connection_valid_) {
				auto ret = game_object_manager_.RemoveGameObject(id_);
				connection_valid_ = false;
				assert(ret == true);
			}
		}

		core::SceneDataForGameObject& GameObjectManagerConnection::RefSceneData() {
			return game_object_manager_.RefSceneData();
		}

	}
}