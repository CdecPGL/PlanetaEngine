#include "collider_component_2d_debug_drawer.hpp"
#include "i_debug_manager.hpp"

#include "..\buildin\c_circle_collider_2d.hpp"
#include "..\buildin\c_straight_line_collider_2d.hpp"


namespace plnt::private_ {
	collider_component_2d_debug_drawer::collider_component_2d_debug_drawer(i_debug_drawer &dd) : debug_drawer_(dd) { }

	bool collider_component_2d_debug_drawer::collide_with(c_circle_collider_2d &circle_collider) {
		const auto pos = circle_collider.get_collision_global_center_position();
		const auto radius = circle_collider.radius() * circle_collider.get_collision_scale();
		debug_drawer_.draw_circle(static_cast<vector_2df>(pos), static_cast<float>(radius), color::red(), false);
		return false;
	}

	bool collider_component_2d_debug_drawer::collide_with([[maybe_unused]] c_straight_line_collider_2d &straight_line_collider) {
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool collider_component_2d_debug_drawer::collide_with([[maybe_unused]] c_polygon_collider_2d &polygon_collider) {
		throw std::logic_error("The method or operation is not implemented.");
	}
}
