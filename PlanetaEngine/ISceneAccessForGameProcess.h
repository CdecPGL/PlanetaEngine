#pragma once

namespace planeta_engine {
	namespace game{
		class IGameObjectManagerAccessor;
		class IGameProcessManagerAccessor;
		class IUIManagerAccessor;
	}
	namespace core {
		class ISceneAccessForGameProcess{
		public:
			virtual ~ISceneAccessForGameProcess() = default;
			virtual game::IGameObjectManagerAccessor& game_object_manager() = 0;
			virtual game::IGameProcessManagerAccessor& game_process_manager() = 0;
			virtual game::IUIManagerAccessor& ui_manager() = 0;
		};
	}
}
