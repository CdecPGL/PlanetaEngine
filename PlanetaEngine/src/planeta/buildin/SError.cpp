#include "..\core\log_utility.hpp"
#include "planeta/core/i_scene.hpp"
#include "..\core\i_task_manager.hpp"
#include "..\core\scene_setup_proxy.hpp"

#include "SError.hpp"
#include "TInstant.hpp"

namespace plnt {
	namespace private_ {
		bool SError::setup_scene(scene_setup_proxy &scene_proxy, const util::parameter_holder &initialize_parameters) {
			auto ip = scene_proxy.scene().task_manager().create_task<TInstant>(task_slot::event_update_phase);
			ip->SetExcuteFunction([]() { PE_LOG_FATAL("エラーが発生しました。"); });
			return true;
		}
	}
}
