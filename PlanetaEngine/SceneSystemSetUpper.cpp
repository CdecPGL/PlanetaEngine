#include "SceneSystemSetUpper.h"
#include "Scene.h"
#include "SceneData.h"
#include "TaskSystemKeyPositions.h"
#include "TaskManager.h"
#include "IGameAccessor.h"

#include "GameObject.h"
#include "TransformComponent.h"

#include "CollisionDetectTask.h"
#include "GameObjectDrawProcessCore.h"
#include "InstantTask.h"

#include "Camera.h"
#include "ScreenDrawer2D.h"
#include "ScreenDrawerGUI.h"

namespace planeta_engine {
	namespace core {

		bool SceneSystemSetUpper::operator()(Scene& scene)
		{
			namespace gp_syskp = task_sys_key_pos;
			auto& scene_data = scene.RefSceneData();
			//ゲームプロセス
			//システムキーポジション設定
			auto& p_mgr = scene.game_process_manager();
			p_mgr.SetKeyPositions(scene.game_accessor().GetTaskKeyPositionList());
			//システムプロセス追加
			//衝突判定プロセス
			auto col_det_proc = p_mgr.AddSystemProcess<system_processes::CollisionDetectTask>(p_mgr.GetKeyPosition(gp_syskp::CollisionDetectProcessKeyPositionID));
			col_det_proc->SetCollisionGroupMatrix(scene.game_accessor().GetCollisionGroupMatrix());
			scene_data.collision_detect_process = col_det_proc;
			//ゲームオブジェクト描画プロセス
			auto godpc = std::make_shared<system_processes::GameObjectDrawProcessCore>();
			auto godp = p_mgr.AddSystemProcess<game_processes::InstantTask>(p_mgr.GetKeyPosition(gp_syskp::GameObjectDrawProcessKeyPositionID));
			godp->SetExcuteFunction([gameobject_draw_process_core = godpc, drawer = scene_data.screen_drawer_2d]{ gameobject_draw_process_core->Update(*drawer); });
			scene_data.draw_component_process_registrator = godpc;
			//ゲームオブジェクト更新プロセス
			auto goup = p_mgr.AddSystemProcess<game_processes::InstantTask>(p_mgr.GetKeyPosition(gp_syskp::GameObjectUpdatetProcessKeyPositionID));
			goup->SetExcuteFunction([&game_object_manager = scene.game_object_manager()]{ game_object_manager.Update(); });
			//UI描画プロセス
			auto uidp = p_mgr.AddSystemProcess<game_processes::InstantTask>(p_mgr.GetKeyPosition(gp_syskp::GUIDrawProcessKeyPositionID));
			uidp->SetExcuteFunction([&ui_manager = scene.gui_manager(),drawer = scene_data.screen_drawer_ui] {ui_manager.Draw(*drawer); });
			//UI更新プロセス
			auto uiup = p_mgr.AddSystemProcess<game_processes::InstantTask>(p_mgr.GetKeyPosition(gp_syskp::GUIUpdateProcessKeyPositionID));
			uiup->SetExcuteFunction([&ui_manager = scene.gui_manager()] {ui_manager.Update(); });
			//Transform関連
			auto tavp = p_mgr.AddSystemProcess<game_processes::InstantTask>(p_mgr.GetKeyPosition(gp_syskp::TransformApplyVelocityProcessKeyPositionID));
			tavp->SetExcuteFunction([&game_object_manager = scene.game_object_manager()]{ game_object_manager.ApplyVelocityToGameObject(); });
			/*auto tclgp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformConvertLocalToGlobalProcessPriority, process::TransformConvertLocalToGlobalProcessName);
			tclgp->SetExcuteFunction([root_transform] {root_transform->ConvertLocalToGlobalRecursively(); });
			auto tcglp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformConvertGlobalToLocalProcessPriority, process::TransformConvertGlobalToLocalProcessName);
			tcglp->SetExcuteFunction([root_transform] {root_transform->ConvertGlobalToLocalRecursively(); });*/
			return true;
		}

	}
}