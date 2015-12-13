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
#include "ScreenDrawerUI.h"

namespace planeta_engine {
	namespace core {

		bool SceneSystemSetUpper::operator()(Scene& scene)
		{
			using namespace system_variables;
			//システムプロセス追加
			//衝突判定プロセス
			auto col_det_proc = scene.game_process_manager().AddSystemProcess<system_processes::CollisionDetectProcess>(process::CollisionDetectProcessPriority, process::CollisionDetectProcessName);
			col_det_proc->SetCollisionGroupMatrix(scene.game_accessor().GetCollisionGroupMatrix());
			//ゲームオブジェクト描画プロセス
			auto godp = scene.game_process_manager().AddSystemProcess<system_processes::GameObjectDrawProcess>(process::GameObjectDrawProcessPriority, process::GameObjectDrawProcessName);
			//ゲームオブジェクト更新プロセス
			auto goup = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::GameObjectUpdatetProcessPriority, process::GameObjectUpdatetProcessName);
			goup->SetExcuteFunction([&game_object_manager = scene.game_object_manager()]{ game_object_manager.Update(); });
			//UI描画プロセス
			auto uidp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::UIDrawProcessPriority, process::UIDrawProcessName);
			uidp->SetExcuteFunction([&ui_manager = scene.ui_manager()] {ui_manager.Draw(); });
			//UI更新プロセス
			auto uiup = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::UIUpdateProcessPriority, process::UIUpdateProcessName);
			uiup->SetExcuteFunction([&ui_manager = scene.ui_manager()] {ui_manager.Update(); });
			//Transform関連
			auto tavp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformApplyVelocityProcessPriority, process::TransformApplyVelocityProcessName);
			tavp->SetExcuteFunction([root_transform = game::GameObject::GetRootTransformComponent(true)]{root_transform->ApplyVelocityRecursively(); });
			/*auto tclgp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformConvertLocalToGlobalProcessPriority, process::TransformConvertLocalToGlobalProcessName);
			tclgp->SetExcuteFunction([root_transform] {root_transform->ConvertLocalToGlobalRecursively(); });
			auto tcglp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformConvertGlobalToLocalProcessPriority, process::TransformConvertGlobalToLocalProcessName);
			tcglp->SetExcuteFunction([root_transform] {root_transform->ConvertGlobalToLocalRecursively(); });*/

			//シーンデータ設定
			std::unique_ptr<SceneData> scene_data = std::make_unique<SceneData>();
			scene_data->collision_detect_process = col_det_proc;
			scene_data->draw_component_process_registrator = godp;
			scene_data->camera = std::make_shared<Camera>();
			scene_data->screen_drawer_2d = std::make_shared<ScreenDrawer2D>(scene.game_accessor().screen());
			scene_data->screen_drawer_ui = std::make_shared<ScreenDrawerUI>(scene.game_accessor().screen());
			scene.SetSceneData(std::move(scene_data));
			return true;
		}

	}
}