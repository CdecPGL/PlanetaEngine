#include "SceneSystemSetUpper.h"
#include "Scene.h"
#include "SceneData.h"
#include "SystemTaskSlot.h"
#include "TaskManager.h"
#include "IGameAccessor.h"
#include "TransformSystem.h"

#include "CollisionWorld.h"
#include "GameObjectDrawSystem.h"
#include "TInstant.h"

namespace planeta_engine {
	namespace core {

		bool SceneSystemSetUpper::operator()(Scene& scene)
		{
			scene.collision_world_->SetCollisionGroupMatrix(scene.game_accessor().GetCollisionGroupMatrix());
			//�Q�[���^�X�N
			auto& t_mgr = *scene.task_manager_;
			//�V�X�e���^�X�N�ǉ�
			//�Փ˔���^�X�N
			auto col_det_proc = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::CollitionDetectPhase);
			col_det_proc->SetExcuteFunction([&col_wor = *scene.collision_world_]{ col_wor.ExcuteCollisionDetection(); });
			//�`��^�X�N
			auto godp = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::DrawPhase);
			godp->SetExcuteFunction([&drw_sys = *scene.gameobject_draw_system_]{ drw_sys.ExcuteDraw(); });
			//�J�����K�p�^�X�N
			auto act = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::ApplyCameraPhase);
			godp->SetExcuteFunction([&drw_sys = *scene.gameobject_draw_system_]{ drw_sys.ApplyCameraState(); });
			//Transform���x�K�p�^�X�N
			auto tavp = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::ApplyVelocityPhase);
			tavp->SetExcuteFunction([&tfm_sys = *scene.transform_system_]{ tfm_sys.ApplyVelocity(); });
			return true;
		}

	}
}