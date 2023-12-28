#pragma once

namespace plnt {
	class CCircleCollider2D;
	class CStraightLineCollider2D;
	class CPolygonCollider2D;

	namespace private_ {
		/*コライダーと衝突するコライダーのインターフェイス*/
		class i_collider_with_collider_2d {
		public:
			i_collider_with_collider_2d() = default;
			i_collider_with_collider_2d(const i_collider_with_collider_2d &) = default;
			i_collider_with_collider_2d(i_collider_with_collider_2d &&) = default;
			// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
			virtual ~i_collider_with_collider_2d() = 0 {}
			i_collider_with_collider_2d &operator=(const i_collider_with_collider_2d &) = default;
			i_collider_with_collider_2d &operator=(i_collider_with_collider_2d &&) = default;

			virtual bool collide_with([[maybe_unused]] CCircleCollider2D &circle_collider) { return false; }

			virtual bool collide_with([[maybe_unused]] CStraightLineCollider2D &straight_line_collider) {
				return false;
			}

			virtual bool collide_with([[maybe_unused]] CPolygonCollider2D &polygon_collider) { return false; }
		};
	}
}
