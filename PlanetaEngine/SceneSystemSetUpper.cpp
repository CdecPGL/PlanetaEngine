#include "SceneSystemSetUpper.h"
#include "Scene.h"
#include "SceneData.h"
#include "SystemVariables.h"
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
			using namespace system_variables;
			auto& scene_data = scene.RefSceneData();
			//システムプロセス追加
			//衝突判定プロセス
			auto col_det_proc = scene.game_process_manager().AddSystemProcess<system_processes::CollisionDetectProcess>(process::CollisionDetectProcessPriority);
			col_det_proc->SetCollisionGroupMatrix(scene.game_accessor().GetCollisionGroupMatrix());
			scene_data.collision_detect_process = col_det_proc;
			//ゲームオブジェクト描画プロセス
			auto godpc = std::make_shared<system_processes::GameObjectDrawProcessCore>();
			auto godp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::GameObjectDrawProcessPriority);
			godp->SetExcuteFunction([gameobject_draw_process_core = godpc, &drawer = *scene_data.screen_drawer_2d]{ gameobject_draw_process_core->Update(drawer); });
			scene_data.draw_component_process_registrator = godpc;
			//ゲームオブジェクト更新プロセス
			auto goup = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::GameObjectUpdatetProcessPriority);
			goup->SetExcuteFunction([&game_object_manager = scene.game_object_manager()]{ game_object_manager.Update(); });
			//UI描画プロセス
			auto uidp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::GUIDrawProcessPriority);
			uidp->SetExcuteFunction([&ui_manager = scene.gui_manager(),&drawer = *scene_data.screen_drawer_ui] {ui_manager.Draw(drawer); });
			//UI更新プロセス
			auto uiup = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::GUIUpdateProcessPriority);
			uiup->SetExcuteFunction([&ui_manager = scene.gui_manager()] {ui_manager.Update(); });
			//Transform関連
			auto tavp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformApplyVelocityProcessPriority);
			tavp->SetExcuteFunction([root_transform = game::GameObject::GetRootTransformComponent(true)]{root_transform->ApplyVelocityRecursively(); });
			/*auto tclgp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformConvertLocalToGlobalProcessPriority, process::TransformConvertLocalToGlobalProcessName);
			tclgp->SetExcuteFunction([root_transform] {root_transform->ConvertLocalToGlobalRecursively(); });
			auto tcglp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformConvertGlobalToLocalProcessPriority, process::TransformConvertGlobalToLocalProcessName);
			tcglp->SetExcuteFunction([root_transform] {root_transform->ConvertGlobalToLocalRecursively(); });*/
			return true;
		}

	}
}