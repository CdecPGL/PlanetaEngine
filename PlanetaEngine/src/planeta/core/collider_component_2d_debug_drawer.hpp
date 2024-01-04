#pragma once

#include "i_collider_with_collider_2d.hpp"

namespace plnt {
	class i_debug_drawer;

	namespace private_ {
		class collider_component_2d_debug_drawer final : public i_collider_with_collider_2d {
		public:
			collider_component_2d_debug_drawer() = delete;
			collider_component_2d_debug_drawer(const collider_component_2d_debug_drawer &) = delete;
			collider_component_2d_debug_drawer(collider_component_2d_debug_drawer &&) = delete;
			~collider_component_2d_debug_drawer() override = default;
			collider_component_2d_debug_drawer &operator=(const collider_component_2d_debug_drawer &) = delete;
			collider_component_2d_debug_drawer &operator=(collider_component_2d_debug_drawer &&) = delete;

			explicit collider_component_2d_debug_drawer(i_debug_drawer &dd);
			bool collide_with(c_circle_collider_2d &circle_collider) override;
			bool collide_with(c_straight_line_collider_2d &straight_line_collider) override;
			bool collide_with(c_polygon_collider_2d &polygon_collider) override;

		private:
			i_debug_drawer &debug_drawer_;
		};
	}
}
