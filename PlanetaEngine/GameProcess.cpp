#include "GameProcess.h"
#include "GameProcessRegistrationData.h"
#include "SceneDataForGameProcess.h"

namespace planeta_engine{
	namespace game{

		void GameProcess::Dispose() {
			disposed(); //�C�x���g���s
			OnDisposed(); //�����̃C�x���g�֐����s
			disposer_(); //�j��
		}

		bool GameProcess::SystemSetUp(const core::GameProcessRegistrationData& resistration_data, const core::SceneDataForGameProcess& special_setup_data) {
			disposer_ = resistration_data.disposer;
			scene_accessor_ = resistration_data.scene_accessor;
			camera_ = special_setup_data.camera;
			return true;
		}

	}
}
