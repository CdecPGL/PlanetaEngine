#include "PolygonColliderComponent.h"

namespace planeta_engine {
	namespace components {

		void PolygonColliderComponent::_ResistToCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc)
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

		void PolygonColliderComponent::_RemoveFromCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc)
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

	}
}