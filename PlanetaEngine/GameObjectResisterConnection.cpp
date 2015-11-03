#include "GameObjectResisterConnection.h"

#include "GameObjectManager.h"

namespace planeta_engine {
	namespace game {

		bool GameObjectResisterConnection::Activate()
		{
			return game_object_manager_.Activate(id_);
		}

		bool GameObjectResisterConnection::InActivate()
		{
			return game_object_manager_.InActivate(id_);
		}

		void GameObjectResisterConnection::Dispose()
		{
			auto ret = game_object_manager_.Remove(id_);
			assert(ret == true);
		}

	}
}