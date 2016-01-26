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
				auto transform = game_object().GetTransformComponent();
				transform->velocity(transform->velocity() + Vector2D<double>(0.0f, -1.0f) * _gravity_scale);
		}

	}
}