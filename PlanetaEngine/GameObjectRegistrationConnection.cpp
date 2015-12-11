#include "GameObjectRegistrationConnection.h"

#include "GameObjectManager.h"

namespace planeta_engine {
	namespace game {

		bool GameObjectRegistrationConnection::Activate()
		{
			return game_object_manager_.Activate(id_);
		}

		bool GameObjectRegistrationConnection::InActivate()
		{
			return game_object_manager_.InActivate(id_);
		}

		void GameObjectRegistrationConnection::Dispose()
		{
			auto ret = game_object_manager_.Remove(id_);
			assert(ret == true);
		}

	}
}