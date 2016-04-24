#include "GameObjectFactory.h"

namespace planeta_engine {
	GameObjectFactory& GameObjectFactory::GetInstance() {
		static GameObjectFactory gof;
		return gof;
	}

	std::shared_ptr<GameObjectBase> GameObjectFactory::CreateGameObject(const std::string& id) {
		
	}

}