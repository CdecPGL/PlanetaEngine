#pragma once
#include "ColliderComponent.h"

namespace planeta_engine {
	namespace components {
		/*ëΩäpå`è’ìÀÉNÉâÉX(ñ¢é¿ëï)*/
		class PolygonColliderComponent : public ColliderComponent {



			virtual void _ResistToCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc) override;

			virtual void _RemoveFromCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc) override;

		};
	}
}
