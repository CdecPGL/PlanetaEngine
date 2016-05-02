#include "SceneSystemSetUpper.h"
#include "Scene.h"
#include "SceneData.h"
#include "SystemTaskSlot.h"
#include "TaskManager.h"
#include "IGameAccessor.h"

#include "GameObject.h"
#include "CTransform2D.h"

#include "CollisionWorld.h"
#include "GameObjectDrawProcessCore.h"
#include "TInstant.h"

#include "Camera.h"
#include "ScreenDrawer2D.h"
#include "ScreenDrawerGUI.h"

namespace planeta_engine {
	namespace core {

		bool SceneSystemSetUpper::operator()(Scene& scene)
		{
			auto& scene_data = scene.RefSceneData();
			//ゲームプロセス
			auto& p_mgr = scene.game_process_manager();
			//システムプロセス追加
			//衝突判定タスク
			auto col_det_proc = p_mgr.AddSystemTask<system_processes::CollisionWorld>(SystemTaskSlot::CollitionDetectPhase);
			col_det_proc->SetCollisionGroupMatrix(scene.game_accessor().GetCollisionGroupMatrix());
			scene_data.collision_world = col_det_proc;
			//描画タスク
			auto godpc = std::make_shared<system_processes::GameObjectDrawProcessCore>();
			auto godp = p_mgr.AddSystemTask<TInstant>(SystemTaskSlot::DrawPhase);
			godp->SetExcuteFunction([gameobject_draw_process_core = godpc, drawer = scene_data.screen_drawer_2d]{ gameobject_draw_process_core->Update(*drawer); });
			scene_data.draw_component_process_registrator = godpc;
			//Transform速度適用タスク
			auto tavp = p_mgr.AddSystemTask<TInstant>(SystemTaskSlot::ApplyVelocityPhase);
			tavp->SetExcuteFunction([] {});
			//Transform座標変換タスク
			/*auto tclgp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformConvertLocalToGlobalProcessPriority, process::TransformConvertLocalToGlobalProcessName);
			tclgp->SetExcuteFunction([root_transform] {root_transform->ConvertLocalToGlobalRecursively(); });
			auto tcglp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformConvertGlobalToLocalProcessPriority, process::TransformConvertGlobalToLocalProcessName);
			tcglp->SetExcuteFunction([root_transform] {root_transform->ConvertGlobalToLocalRecursively(); });*/
			return true;
		}

	}
}