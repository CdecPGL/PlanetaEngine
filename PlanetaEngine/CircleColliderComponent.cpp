#include "CircleColliderComponent.h"
#include "CollisionDetectProcess.h"

namespace planeta_engine {
	namespace components {

		void CircleColliderComponent::_ResistToCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc)
		{
			col_det_proc.Resist(std::static_pointer_cast<CircleColliderComponent>(this_shared()));
		}

		void CircleColliderComponent::_RemoveFromCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc)
		{
			col_det_proc.Remove(std::static_pointer_cast<CircleColliderComponent>(this_shared()));
		}

	}
}