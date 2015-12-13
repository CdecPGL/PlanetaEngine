#include "GameObjectManagerConnection.h"

#include "GameObjectManager.h"

namespace planeta_engine {
	namespace game {

		bool GameObjectManagerConnection::Activate()
		{
			return game_object_manager_.Activate(id_);
		}

		bool GameObjectManagerConnection::InActivate()
		{
			return game_object_manager_.InActivate(id_);
		}

		void GameObjectManagerConnection::Dispose()
		{
			auto ret = game_object_manager_.Remove(id_);
			assert(ret == true);
		}

		core::SceneDataForGameObject& GameObjectManagerConnection::RefSceneData() {
			return game_object_manager_.RefSceneData();
		}

	}
}