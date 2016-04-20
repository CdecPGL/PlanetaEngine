#include "Task.h"
#include "TaskRegistrationData.h"
#include "SceneDataForTask.h"
#include "TaskPosition.h"

namespace planeta_engine{
	namespace game{

		Task::~Task() = default;

		void Task::Dispose() {
			disposed(); //イベント実行
			OnDisposed(); //自分のイベント関数実行
			disposer_(); //破棄
		}

		bool Task::SystemSetUpAndInitialize(const core::TaskRegistrationData& resistration_data, const core::SceneDataForTask& special_setup_data) {
			disposer_ = resistration_data.disposer;
			scene_accessor_ = resistration_data.scene_accessor;
			gameprocess_position_ = std::make_unique<core::TaskPosition>(resistration_data.position_in_list);
			return OnCreated();
		}

		const core::TaskPosition& Task::game_process_position() const {
			return *gameprocess_position_;
		}

	}
}
