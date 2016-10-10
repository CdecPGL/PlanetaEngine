#pragma once

#include "MacroUtility.h"

namespace planeta {
	class CCircleCollider2D;
	class CStraightLineCollider2D;
	class CPolygonCollider2D;
	namespace private_ {
		/*コライダーと衝突するコライダーのインターフェイス*/
		class IColliderWithCollider2D {
		public:
			virtual bool CollideWith(CCircleCollider2D& circle_collider) { PE_UNREFERENCED_PARAMETER(circle_collider); return false; }
			virtual bool CollideWith(CStraightLineCollider2D& straight_line_collider) { PE_UNREFERENCED_PARAMETER(straight_line_collider); return false; }
			virtual bool CollideWith(CPolygonCollider2D& polygon_collider) { PE_UNREFERENCED_PARAMETER(polygon_collider); return false; }
		};
	}
}