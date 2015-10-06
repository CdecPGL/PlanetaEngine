#pragma once

namespace planeta_engine {
	namespace game {
		class IGameProcessManagerAccessor;
		class IUIManagerAccessor;
	}
	namespace core {
		class ISceneAccessForUI {
		public:
			virtual ~ISceneAccessForUI() = default;
			virtual game::IGameProcessManagerAccessor& game_process_manager() = 0;
			virtual game::IUIManagerAccessor& ui_manager() = 0;
		};
	}
}