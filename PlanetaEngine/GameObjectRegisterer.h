#pragma once

namespace planeta_engine {
	template<const char* GameObjectID, class GameObjectType>
	class GameObjectRegisterer {
	public:
		GameObjectRegisterer() {

		}
	};
}

#define PE_REGISTER_GAMEOBJECT(id, type)\
static GameObjectRegisterer<id, type> gameobject_registerer;