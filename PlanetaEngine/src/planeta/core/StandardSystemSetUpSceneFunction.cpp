#include "StandardSceneManagerUtility.hpp"

#include "Scene.hpp"
#include "TaskManager.hpp"
#include "collision_world.hpp"
#include "draw_system.hpp"
#include "TransformSystem.hpp"

#include "planeta/buildin/TInstant.hpp"

namespace plnt {
	namespace private_ {
		bool StandardSystemSetUpScene(Scene &scene) {
			//ゲームタスク
			auto &t_mgr = *scene.task_manager_internal_pointer();
			//システムタスク追加
			//衝突判定タスク
			auto col_det_proc = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::CollitionDetectPhase);
			col_det_proc->SetExcuteFunction([&col_wor = *scene.collision_world_internal_pointer()] {
				col_wor.execute_collision_detection();
			});
			//描画タスク
			auto godp = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::DrawPhase);
			godp->SetExcuteFunction([&drw_sys = *scene.draw_system_internal_pointer()] { drw_sys.execute_draw(); });
			auto guidp = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::DrawGUIPhase);
			guidp->SetExcuteFunction([&drw_sys = *scene.draw_system_internal_pointer()] { drw_sys.execute_draw_gui(); });
			//カメラ適用タスク
			auto act = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::ApplyCameraPhase);
			act->SetExcuteFunction([&drw_sys = *scene.draw_system_internal_pointer()] { drw_sys.apply_camera_state(); });
			//Transform速度適用タスク
			auto tavp = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::ApplyVelocityPhase);
			tavp->SetExcuteFunction(
				[&tfm_sys = *scene.transform_system_internal_pointer()] { tfm_sys.ApplyVelocity(); });
			return true;
		}
	}
}
