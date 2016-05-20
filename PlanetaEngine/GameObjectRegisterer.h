#pragma once

#include "GameObjectFactory.h"

namespace planeta_engine {
	template<const char* GameObjectID, class GameObjectType>
	class GameObjectRegisterer {
	public:
		GameObjectRegisterer() {
			GameObjectFactory::GetInstance().RegisterGameObject<GameObjectType>(GameObjectID);
		}
	};
}

//GameObject���V�X�e���ɓo�^����(ID,�^)
#define PE_REGISTER_GAMEOBJECT(id, type)\
namespace {GameObjectRegisterer<id,type> gameobject_registerer();}