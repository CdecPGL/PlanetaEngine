#include "CBelongingGroundGravity.h"

#include "IGameObjectForComponent.h"
#include "CTransform2D.h"
#include "CGround2D.h"
#include "TGInstant.h"

namespace planeta {
	bool CBelongingGroundGravity::OnInitialized() {
		transform2d_.reset(game_object().GetComponent<CTransform2D>());
		auto proc = game_object().CreateAndAttachTask<TGInstant>(TaskSlot::PreCollisionEarlyPhase);
		proc->SetExcuteFunction([this]() {Update(); });
		return true;
	}

	void CBelongingGroundGravity::Update() {
		transform2d_->GroundAccelerate(Vector2Dd(0.0f, -1.0f) * _gravity_scale);
	}
}