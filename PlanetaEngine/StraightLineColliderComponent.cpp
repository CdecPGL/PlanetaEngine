#include "StraightLineColliderComponent.h"
#include "CollisionDetectProcess.h"

namespace planeta_engine {
	namespace components {

		void StraightLineColliderComponent::_ResistToCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc)
		{
			col_det_proc.Resist(std::static_pointer_cast<StraightLineColliderComponent>(this_shared()));
		}

		void StraightLineColliderComponent::_RemoveFromCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc)
		{
			col_det_proc.Remove(std::static_pointer_cast<StraightLineColliderComponent>(this_shared()));
		}

	}
}