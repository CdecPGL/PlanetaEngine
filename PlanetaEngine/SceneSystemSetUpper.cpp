#include "SceneSystemSetUpper.h"
#include "Scene.h"
#include "SceneData.h"
#include "SystemVariables.h"
#include "GameProcessManager.h"
#include "IGameAccessor.h"

#include "GameObject.h"
#include "TransformComponent.h"

#include "CollisionDetectProcess.h"
#include "GameObjectDrawProcess.h"
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
			auto godp = scene.game_process_manager().AddSystemProcess<system_processes::GameObjectDrawProcess>(process::GameObjectDrawProcessPriority);
			scene_data.draw_component_process_registrator = godp;
			//ゲームオブジェクト更新プロセス
			auto goup = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::GameObjectUpdatetProcessPriority);
			goup->SetExcuteFunction([&game_object_manager = scene.game_object_manager()]{ game_object_manager.Update(); });
			//UI描画プロセス
			auto uidp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::UIDrawProcessPriority);
			uidp->SetExcuteFunction([&ui_manager = scene.gui_manager()] {ui_manager.Draw(); });
			//UI更新プロセス
			auto uiup = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::UIUpdateProcessPriority);
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