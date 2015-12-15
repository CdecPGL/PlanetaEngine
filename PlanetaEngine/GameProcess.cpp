#include "GameProcess.h"
#include "GameProcessRegistrationData.h"
#include "SceneDataForGameProcess.h"

namespace planeta_engine{
	namespace game{

		GameProcess::~GameProcess() = default;

		void GameProcess::Dispose() {
			disposed(); //�C�x���g���s
			OnDisposed(); //�����̃C�x���g�֐����s
			disposer_(); //�j��
		}

		bool GameProcess::SystemSetUpAndInitialize(const core::GameProcessRegistrationData& resistration_data, const core::SceneDataForGameProcess& special_setup_data) {
			disposer_ = resistration_data.disposer;
			scene_accessor_ = resistration_data.scene_accessor;
			position_in_list_ = std::make_unique<core::GameProcessPositionInList>(resistration_data.position_in_list);
			return OnCreated();
		}

	}
}
