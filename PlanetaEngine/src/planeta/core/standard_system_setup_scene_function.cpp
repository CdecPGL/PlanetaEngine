#include "standard_scene_manager_utility.hpp"
#include "scene.hpp"
#include "task_manager.hpp"
#include "collision_world.hpp"
#include "draw_system.hpp"
#include "transform_system.hpp"
#include "planeta/buildin/TInstant.hpp"

namespace plnt::private_ {
	bool standard_system_set_up_scene(scene &scene) {
		//ゲームタスク
		auto &t_mgr = *scene.task_manager_internal_pointer();
		//システムタスク追加
		//衝突判定タスク
		const auto col_det_proc = t_mgr.add_system_task<t_instant>(system_task_slot::collision_detect_phase);
		col_det_proc->set_execute_function([&col_wor = *scene.collision_world_internal_pointer()] {
			col_wor.execute_collision_detection();
		});
		//描画タスク
		const auto go_draw_task = t_mgr.add_system_task<t_instant>(system_task_slot::draw_phase);
		go_draw_task->set_execute_function([&drw_sys = *scene.draw_system_internal_pointer()] { drw_sys.execute_draw(); });
		const auto gui_draw_task = t_mgr.add_system_task<t_instant>(system_task_slot::draw_gui_phase);
		gui_draw_task->set_execute_function([&drw_sys = *scene.draw_system_internal_pointer()] {
			drw_sys.execute_draw_gui();
		});
		//カメラ適用タスク
		const auto act = t_mgr.add_system_task<t_instant>(system_task_slot::apply_camera_phase);
		act->set_execute_function([&drw_sys = *scene.draw_system_internal_pointer()] { drw_sys.apply_camera_state(); });
		//Transform速度適用タスク
		const auto transform_task = t_mgr.add_system_task<t_instant>(system_task_slot::apply_velocity_phase);
		transform_task->set_execute_function(
			[&tfm_sys = *scene.transform_system_internal_pointer()] { tfm_sys.apply_velocity(); });
		return true;
	}
}
