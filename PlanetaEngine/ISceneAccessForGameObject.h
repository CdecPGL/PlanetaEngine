#pragma once


#pragma once

namespace planeta_engine {
	namespace game {
		class IGameObjectManagerAccessor;
		class IGameProcessManagerAccessor;
	}
	namespace core {
		class ISceneAccessForGameObject {
		public:
			virtual ~ISceneAccessForGameObject() = default;
			virtual game::IGameObjectManagerAccessor& game_object_manager() = 0;
			virtual game::IGameProcessManagerAccessor& game_process_manager() = 0;
		};
	}
}