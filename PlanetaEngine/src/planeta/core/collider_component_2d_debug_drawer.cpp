#include "collider_component_2d_debug_drawer.hpp"
#include "i_debug_manager.hpp"

#include "planeta/buildin/CCircleCollider2D.hpp"
#include "planeta/buildin/CStraightLineCollider2D.hpp"


namespace plnt::private_ {
	collider_component_2d_debug_drawer::collider_component_2d_debug_drawer(i_debug_drawer &dd) : debug_drawer_(dd) { }

	bool collider_component_2d_debug_drawer::collide_with(CCircleCollider2D &circle_collider) {
		const auto pos = circle_collider.GetCollisionGlobalCenterPosition();
		const auto radius = circle_collider.radius() * circle_collider.GetCollisionScale();
		debug_drawer_.draw_circle(static_cast<Vector2Df>(pos), static_cast<float>(radius), color::red(), false);
		return false;
	}

	bool collider_component_2d_debug_drawer::collide_with(CStraightLineCollider2D &straight_line_collider) {
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool collider_component_2d_debug_drawer::collide_with(CPolygonCollider2D &polygon_collider) {
		throw std::logic_error("The method or operation is not implemented.");
	}
}
