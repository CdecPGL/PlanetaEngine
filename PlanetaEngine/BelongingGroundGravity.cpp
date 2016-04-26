#include "CBelongingGroundGravity.h"

#include "IGameObjectAccessor.h"
#include "CTransform2D.h"
#include "CGround2D.h"

namespace planeta_engine {
	bool CBelongingGroundGravity::OnInitialized() {
		return true;
	}

	void CBelongingGroundGravity::OnUpdated() {
		auto& transform = game_object().transform();
		transform.Accelerate(Vector2Dd(0.0f, -1.0f) * _gravity_scale);
	}
}