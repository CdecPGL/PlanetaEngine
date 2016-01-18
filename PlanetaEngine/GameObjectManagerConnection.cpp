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
			auto ret = game_object_manager_.RemoveGameObject(id_);
			assert(ret == true);
		}

		core::SceneDataForGameObject& GameObjectManagerConnection::RefSceneData() {
			return game_object_manager_.RefSceneData();
		}

	}
}