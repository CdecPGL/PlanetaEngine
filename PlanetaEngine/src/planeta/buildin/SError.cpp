#include "../core/log_utility.hpp"
#include "planeta/core/i_scene.hpp"
#include "../core/i_task_manager.hpp"
#include "../core/scene_setup_proxy.hpp"

#include "SError.hpp"
#include "TInstant.hpp"

namespace plnt::private_ {
	bool s_error::setup_scene(scene_setup_proxy &scene_proxy, const util::parameter_holder &initialize_parameters) {
		const auto ip = scene_proxy.scene().task_manager().create_task<t_instant>(task_slot::event_update_phase);
		ip->set_execute_function([]() { PE_LOG_FATAL("エラーが発生しました。"); });
		return true;
	}
}
