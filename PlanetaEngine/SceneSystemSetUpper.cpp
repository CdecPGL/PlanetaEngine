#include "SceneSystemSetUpper.h"
#include "Scene.h"
#include "SystemVariables.h"
#include "GameProcessManager.h"
#include "IGameAccessor.h"

#include "GameObject.h"
#include "TransformComponent.h"

#include "CollisionDetectProcess.h"
#include "GameObjectDrawProcess.h"
#include "GameObjectUpdateProcess.h"
#include "UIDrawProcess.h"
#include "UIUpdateProcess.h"
#include "InstanceProcess.h"

namespace planeta_engine {
	namespace core {

		bool SceneSystemSetUpper::operator()(Scene& scene)
		{
			using namespace system_variables;
			//�V�X�e���v���Z�X�ǉ�
			//�Փ˔���v���Z�X
			auto col_det_proc = scene.game_process_manager().AddSystemProcess<system_processes::CollisionDetectProcess>(process::CollisionDetectProcessPriority, process::CollisionDetectProcessName);
			col_det_proc->SetCollisionGroupMatrix(scene.game_accessor().GetCollisionGroupMatrix());
			//�Q�[���I�u�W�F�N�g�`��v���Z�X
			scene.game_process_manager().AddSystemProcess<system_processes::GameObjectDrawProcess>(process::GameObjectDrawProcessPriority, process::GameObjectDrawProcessName);
			//�Q�[���I�u�W�F�N�g�X�V�v���Z�X
			scene.game_process_manager().AddSystemProcess<system_processes::GameObjectUpdateProcess>(process::GameObjectUpdatetProcessPriority, process::GameObjectUpdatetProcessName);
//			auto goup = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::GameObjectUpdatetProcessPriority, process::GameObjectUpdatetProcessName);
//			goup->SetExcuteFunction([&game_object_manager = scene.game_object_manager()]{ game_object_manager.Update(); });
			//UI�`��v���Z�X
			auto uidp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::UIDrawProcessPriority, process::UIDrawProcessName);
			uidp->SetExcuteFunction([&ui_manager = scene.ui_manager()] {ui_manager.Draw(); });
			//UI�X�V�v���Z�X
			auto uiup = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::UIUpdateProcessPriority, process::UIUpdateProcessName);
			uiup->SetExcuteFunction([&ui_manager = scene.ui_manager()] {ui_manager.Update(); });
			/*scene.game_process_manager().AddSystemProcess<system_processes::UIDrawProcess>(process::UIDrawProcessPriority, process::UIDrawProcessName);
			scene.game_process_manager().AddSystemProcess<system_processes::UIUpdateProcess>(process::UIUpdateProcessPriority, process::UIUpdateProcessName);*/
			//Transform�֘A
			auto root_transform = game::GameObject::GetRootTransformComponent(true);
			auto tavp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformApplyVelocityProcessPriority, process::TransformApplyVelocityProcessName);
			tavp->SetExcuteFunction([root_transform]{root_transform->ApplyVelocityRecursively(); });
			/*auto tclgp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformConvertLocalToGlobalProcessPriority, process::TransformConvertLocalToGlobalProcessName);
			tclgp->SetExcuteFunction([root_transform] {root_transform->ConvertLocalToGlobalRecursively(); });
			auto tcglp = scene.game_process_manager().AddSystemProcess<game_processes::InstantProcess>(process::TransformConvertGlobalToLocalProcessPriority, process::TransformConvertGlobalToLocalProcessName);
			tcglp->SetExcuteFunction([root_transform] {root_transform->ConvertGlobalToLocalRecursively(); });*/
			return true;
		}

	}
}