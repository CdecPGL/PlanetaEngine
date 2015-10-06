#pragma once

#include "ColliderComponent.h"

namespace planeta_engine {
	namespace components {
		class StraightLineColliderComponent : public ColliderComponent {
		public:
			const double length()const { return length_; }
			void length(double l) { length_ = l; }
		private:
			virtual void _ResistToCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc) override;
			virtual void _RemoveFromCollisionDetectProcessByType(system_processes::CollisionDetectProcess& col_det_proc) override;
			double length_ = 1.0;
		};
	}
}
