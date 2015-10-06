#pragma once

namespace planeta_engine {
	namespace game {
		class IGameObjectManagerSetup;
		class IGameProcessManagerSetup;
		class IUIManagerSetup;
	}
	namespace core {
		/*�V�[���̐ݒ�C���^�[�t�F�X*/
		class ISceneSetup {
		public:
			virtual ~ISceneSetup() = 0 {};
			virtual game::IGameObjectManagerSetup& game_object_manager() = 0;
			virtual game::IGameProcessManagerSetup& game_process_manager() = 0;
			virtual game::IUIManagerSetup& ui_manager() = 0;
		};
	}
}
