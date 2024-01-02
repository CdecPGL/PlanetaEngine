#include "StandardSceneManagerUtility.hpp"

#include "scene.hpp"
#include "task_manager.hpp"
#include "collision_world.hpp"
#include "draw_system.hpp"
#include "transform_system.hpp"

#include "planeta/buildin/TInstant.hpp"

namespace plnt {
	namespace private_ {
		bool StandardSystemSetUpScene(scene &scene) {
			//ゲームタスク
			auto &t_mgr = *scene.task_manager_internal_pointer();
			//システムタスク追加
			//衝突判定タスク
			auto col_det_proc = t_mgr.add_system_task<TInstant>(system_task_slot::collision_detect_phase);
			col_det_proc->SetExcuteFunction([&col_wor = *scene.collision_world_internal_pointer()] {
				col_wor.execute_collision_detection();
			});
			//描画タスク
			auto godp = t_mgr.add_system_task<TInstant>(system_task_slot::draw_phase);
			godp->SetExcuteFunction([&drw_sys = *scene.draw_system_internal_pointer()] { drw_sys.execute_draw(); });
			auto guidp = t_mgr.add_system_task<TInstant>(system_task_slot::draw_gui_phase);
			guidp->SetExcuteFunction([&drw_sys = *scene.draw_system_internal_pointer()] { drw_sys.execute_draw_gui(); });
			//カメラ適用タスク
			auto act = t_mgr.add_system_task<TInstant>(system_task_slot::apply_camera_phase);
			act->SetExcuteFunction([&drw_sys = *scene.draw_system_internal_pointer()] { drw_sys.apply_camera_state(); });
			//Transform速度適用タスク
			auto tavp = t_mgr.add_system_task<TInstant>(system_task_slot::apply_velocity_phase);
			tavp->SetExcuteFunction(
				[&tfm_sys = *scene.transform_system_internal_pointer()] { tfm_sys.apply_velocity(); });
			return true;
		}
	}
}
