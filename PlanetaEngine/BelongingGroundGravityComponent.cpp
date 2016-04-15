#include "BelongingGroundGravityComponent.h"

#include "IGameObjectAccessor.h"
#include "TransformComponent.h"
#include "GroundComponent.h"

namespace planeta_engine {
	namespace components {

		bool BelongingGroundGravityComponent::OnInitialized()
		{
			return true;
		}

		void BelongingGroundGravityComponent::OnUpdated()
		{
			auto& transform = game_object().transform();
			transform.Accelerate(Vector2Dd(0.0f, -1.0f) * _gravity_scale);
		}

	}
}