#pragma once

namespace planeta {
	class CCircleCollider2D;
	class CStraightLineCollider2D;
	class CPolygonCollider2D;
	namespace core {
		/*コライダーと衝突するコライダーのインターフェイス*/
		class IColliderWithCollider2D {
		public:
			virtual bool CollideWith(CCircleCollider2D& circle_collider) { return false; }
			virtual bool CollideWith(CStraightLineCollider2D& straight_line_collider) { return false; }
			virtual bool CollideWith(CPolygonCollider2D& polygon_collider) { return false; }
		};
	}
}