#include "..\core\log_utility.hpp"
#include "planeta/core/i_scene.hpp"
#include "..\core\i_task_manager.hpp"
#include "planeta/core/SceneSetUpProxy.hpp"

#include "SError.hpp"
#include "TInstant.hpp"

namespace plnt {
	namespace private_ {
		bool SError::SetUpScene(SceneSetUpProxy &scene_proxy, const util::parameter_holder &initialize_parameters) {
			auto ip = scene_proxy.scene().task_manager().create_task<TInstant>(TaskSlot::EventUpdatePhase);
			ip->SetExcuteFunction([]() { PE_LOG_FATAL("エラーが発生しました。"); });
			return true;
		}
	}
}
