#pragma once

#include "ColliderComponent.h"

namespace planeta_engine {
	namespace components {
		class CircleColliderComponent : public ColliderComponent {
		public:
			const double radius()const { return _radius; }
			void radius(double r) { _radius = r; }
		private:
			virtual void _ResistToCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc) override;
			virtual void _RemoveFromCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc) override;

			double _radius = 1.0;
		};
	}
}
