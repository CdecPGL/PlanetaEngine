#pragma once

namespace plnt {
	class c_circle_collider_2d;
	class c_straight_line_collider_2d;
	class c_polygon_collider_2d;

	namespace private_ {
		/*コライダーと衝突するコライダーのインターフェイス*/
        // NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
		class i_collider_with_collider_2d {
		public:
			// NOLINTNEXTLINE(clang-diagnostic-microsoft-pure-definition,modernize-use-equals-default)
			virtual ~i_collider_with_collider_2d() = 0 {}

			virtual bool collide_with([[maybe_unused]] c_circle_collider_2d &circle_collider) { return false; }

			virtual bool collide_with([[maybe_unused]] c_straight_line_collider_2d &straight_line_collider) {
				return false;
			}

			virtual bool collide_with([[maybe_unused]] c_polygon_collider_2d &polygon_collider) { return false; }
		};
	}
}
