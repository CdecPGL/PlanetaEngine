#include "CBelongingGroundGravity.h"

#include "IGameObject.h"
#include "CTransform2D.h"
#include "CGround2D.h"
#include "TInstant.h"

namespace planeta {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CBelongingGroundGravity) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CBelongingGroundGravity, gravity_scale)
			.ShallowCopyTarget(&CBelongingGroundGravity::_gravity_scale);
	}

	bool CBelongingGroundGravity::GetOtherComponentsProc(const GOComponentGetter& com_getter) {
		if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
		transform2d_.reset(com_getter.GetComponent<CTransform2D>());
		return true;
	}

	void CBelongingGroundGravity::OnInitialized() {
		Super::OnInitialized();
		auto proc = game_object().CreateAndAttachTask<TInstant>(TaskSlot::PreCollisionEarlyPhase);
		proc->SetExcuteFunction([this]() {Update(); });
	}

	void CBelongingGroundGravity::Update() {
		transform2d_->GroundAccelerate(Vector2Dd(0.0f, -1.0f) * _gravity_scale);
	}
}