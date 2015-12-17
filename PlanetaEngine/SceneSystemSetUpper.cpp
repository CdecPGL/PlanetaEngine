#include "SceneSystemSetUpper.h"
#include "Scene.h"
#include "SceneData.h"
#include "GameProcessSystemKeyPositions.h"
#include "GameProcessManager.h"
#include "IGameAccessor.h"

#include "GameObject.h"
#include "TransformComponent.h"

#include "CollisionDetectProcess.h"
#include "GameObjectDrawProcessCore.h"
#include "InstanceProcess.h"

#include "Camera.h"
#include "ScreenDrawer2D.h"
#include "ScreenDrawerGUI.h"

namespace planeta_engine {
	namespace core {

		bool SceneSystemSetUpper::operator()(Scene& scene)
		{
			namespace gp_syskp = gameprocess_sys_key_pos;
			auto& scene_data = scene.RefSceneData();
			//ゲームプロセス
			//システムキーポジション追加
			auto& p_mgr = scene.game_process_manager();
			p_mgr.CreateSystemKeyPosition({ 
				gp_syskp::GameObjectUpdatetProcessKeyPositionID,
				gp_syskp::TransformApplyVelocityProcessKeyPositionID,
				gp_syskp::CollisionDetectProcessKeyPositionID,
				gp_syskp::GameObjectDrawProcessKeyPositionID,
				gp_syskp::GUIUpdateProcessKeyPositionID,
				gp_syskp::GUIDrawProcessKeyPositionID,
			});
			//システムプロセス追加
			//衝突判定プロセス
			auto col_det_proc = p_mgr.AddSystemProcess<system_processes::CollisionDetectProcess>(p_mgr.GetKeyPosition(gp_syskp::CollisionDetectProcessKeyPositionID), InsertPosIndication::At);
			col_det_proc->SetCollisionGroupMatrix(scene.game_accessor().GetCollisionGroupMatrix());
			scene_data.collision_detect_process = col_det_proc;
			//ゲームオブジェクト描画プロセス
			auto godpc = std::make_shared<system_processes::GameObjectDrawProcessCore>();
			auto godp = p_mgr.AddSystemProcess<game_processes::InstantProcess>(p_mgr.GetKeyPosition(gp_syskp::GameObjectDrawProcessKeyPositionID), InsertPosIndication::At);
			godp->SetExcuteFunction([gameobject_draw_process_core = godpc, drawer = scene_data.screen_drawer_2d]{ gameobject_draw_process_core->Update(*drawer); });
			scene_data.draw_component_process_registrator = godpc;
			//ゲームオブジェクト更新プロセス
			auto goup = p_mgr.AddSystemProcess<game_processes::InstantProcess>(p_mgr.GetKeyPosition(gp_syskp::GameObjectUpdatetProcessKeyPositionID), InsertPosIndication::At);
			goup->SetExcuteFunction([&game_object_manager = scene.game_object_manager()]{ game_object_manager.Update(); });
			//UI描画プロセス
			auto uidp = p_mgr.AddSystemProcess<game_processes::InstantProcess>(p_mgr.GetKeyPosition(gp_syskp::GUIDrawProcessKeyPositionID), InsertPosIndication::At);
			uidp->SetExcuteFunction([&ui_manager = scene.gui_manager(),drawer = scene_data.screen_drawer_ui] {ui_manager.Draw(*drawer); });
			//UI更新プロセス
			auto uiup = p_mgr.AddSystemProcess<game_processes::InstantProcess>(p_mgr.GetKeyPosition(gp_syskp::GUIUpdateProcessKeyPositionID), InsertPosIndication::At);
			uiup->SetExcuteFunction([&ui_manager = scene.gui_manager()] {ui_manager.Update(); });
			//Transform関連
			auto tavp = p_mgr.AddSystemProcess<game_processes::InstantProcess>(p_mgr.GetKeyPosition(gp_syskp::TransformApplyVelocityProcessKeyPositionID), InsertPosIndication::At);
			tavp->SetExcuteFunction([root_transform = game::GameObject::GetRootTransformComponent(true)]{root_transform->ApplyVelocityRecursively(); });
			/*auto tclgp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformConvertLocalToGlobalProcessPriority, process::TransformConvertLocalToGlobalProcessName);
			tclgp->SetExcuteFunction([root_transform] {root_transform->ConvertLocalToGlobalRecursively(); });
			auto tcglp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformConvertGlobalToLocalProcessPriority, process::TransformConvertGlobalToLocalProcessName);
			tcglp->SetExcuteFunction([root_transform] {root_transform->ConvertGlobalToLocalRecursively(); });*/
			return true;
		}

	}
}