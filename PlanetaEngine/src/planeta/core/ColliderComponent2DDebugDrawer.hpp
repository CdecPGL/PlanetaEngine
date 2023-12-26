#pragma once

#include "IColliderWithCollider2D.hpp"

namespace plnt {
	class IDebugDrawer;

	namespace private_ {
		class ColliderComponent2DDebugDrawer : public IColliderWithCollider2D {
		public:
			ColliderComponent2DDebugDrawer(IDebugDrawer &dd);
			bool CollideWith(CCircleCollider2D &circle_collider) override;
			bool CollideWith(CStraightLineCollider2D &straight_line_collider) override;
			bool CollideWith(CPolygonCollider2D &polygon_collider) override;

		private:
			IDebugDrawer &debug_drawer_;
		};
	}
}
