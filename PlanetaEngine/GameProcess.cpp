#include "GameProcess.h"
#include "GameProcessRegistrationData.h"
#include "SceneDataForGameProcess.h"
#include "GameProcessPosition.h"

namespace planeta_engine{
	namespace game{

		GameProcess::~GameProcess() = default;

		void GameProcess::Dispose() {
			disposed(); //イベント実行
			OnDisposed(); //自分のイベント関数実行
			disposer_(); //破棄
		}

		bool GameProcess::SystemSetUpAndInitialize(const core::GameProcessRegistrationData& resistration_data, const core::SceneDataForGameProcess& special_setup_data) {
			disposer_ = resistration_data.disposer;
			scene_accessor_ = resistration_data.scene_accessor;
			gameprocess_position_ = std::make_unique<core::GameProcessPosition>(resistration_data.position_in_list);
			return OnCreated();
		}

		const core::GameProcessPosition& GameProcess::game_process_position() const {
			return *gameprocess_position_;
		}

	}
}
