#include "planeta/core/StandardSceneManagerUtility.hpp"

#include "Scene.hpp"
#include "TaskManager.hpp"
#include "planeta/core/CollisionWorld.hpp"
#include "planeta/core/DrawSystem.hpp"
#include "planeta/core/TransformSystem.hpp"

#include "planeta/buildin/TInstant.hpp"

namespace plnt {
	namespace private_ {
		bool StandardSystemSetUpScene(Scene& scene) {
			//�Q�[���^�X�N
			auto& t_mgr = *scene.task_manager_internal_pointer();
			//�V�X�e���^�X�N�ǉ�
			//�Փ˔���^�X�N
			auto col_det_proc = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::CollitionDetectPhase);
			col_det_proc->SetExcuteFunction([&col_wor = *scene.collision_world_internal_pointer()]{ col_wor.ExcuteCollisionDetection(); });
			//�`��^�X�N
			auto godp = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::DrawPhase);
			godp->SetExcuteFunction([&drw_sys = *scene.draw_system_internal_pointer()]{ drw_sys.ExcuteDraw(); });
			auto guidp = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::DrawGUIPhase);
			guidp->SetExcuteFunction([&drw_sys = *scene.draw_system_internal_pointer()]{ drw_sys.ExcuteDrawGUI(); });
			//�J�����K�p�^�X�N
			auto act = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::ApplyCameraPhase);
			act->SetExcuteFunction([&drw_sys = *scene.draw_system_internal_pointer()]{ drw_sys.ApplyCameraState(); });
			//Transform���x�K�p�^�X�N
			auto tavp = t_mgr.AddSystemTask<TInstant>(SystemTaskSlot::ApplyVelocityPhase);
			tavp->SetExcuteFunction([&tfm_sys = *scene.transform_system_internal_pointer()]{ tfm_sys.ApplyVelocity(); });
			return true;
		}
	}
}