#include "BelongingGroundGravityComponent.h"

#include "IGameObjectAccessor.h"
#include "TransformComponent.h"
#include "GroundComponent.h"

namespace planeta_engine {
	namespace components {

		bool BelongingGroundGravityComponent::Initialize_()
		{
			return true;
		}

		void BelongingGroundGravityComponent::Update_()
		{
			auto bgc = game_object()->GetBelongingGroundComponent();
			if (bgc) {
				auto transform = game_object()->GetTransformComponent();
				transform->velocity(transform->velocity() + bgc->GetDownwardDirectionVector(transform->global_position())*_gravity_scale);
			}
		}

	}
}