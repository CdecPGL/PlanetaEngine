#include "../core/i_game_object.hpp"
#include "../core/log_utility.hpp"
#include "../math/math_constant.hpp"

#include "CPlanetGround2D.hpp"
#include "CTransform2D.hpp"
#include "CCircleCollider2D.hpp"
#include "CPlanet.hpp"

namespace plnt {
	bool c_planet_ground_2d::get_other_components_proc(const go_component_getter &com_getter) {
		if (!c_ground_2d::get_other_components_proc(com_getter)) { return false; };
		planet_component_.reset(com_getter.get_component<c_planet>());
		if (!planet_component_) {
			PE_LOG_ERROR("PlanetComponentを取得できませんでした。");
			return false;
		}
		return true;
	}

	bool c_planet_ground_2d::collide_with(c_circle_collider_2d &collider) {
		//			TransformComponent& transform = game_object().transform();
		const auto collider_pos = collider.get_collision_global_center_position();
		//コライダーの惑星座標を求める
		//衝突判定
		if (const auto collider_ground_pos = convert_position_global_to_ground(collider_pos); planet_component_->
			get_height_by_rad(collider_ground_pos.x) + collider.radius() * collider.get_collision_scale()
			> collider_ground_pos.y) {
			auto &collider_transform = collider.transform2d();
			//速度の修正
			collider_transform.ground_velocity({collider_transform.ground_velocity().x, 0});
			//押し出し
			collider_transform.ground_position({
				collider_transform.ground_position().x,
				planet_component_->get_height_by_rad(collider_transform.ground_position().x) + collider.radius() *
				collider
				.get_collision_scale()
			});
			return true;
		}
		return false;
	}

	double c_planet_ground_2d::get_altitude_with_ground_position(const vector_2dd &ground_pos) const {
		return ground_pos.y - planet_component_->get_height_by_rad(ground_pos.x);
	}

	vector_2dd c_planet_ground_2d::convert_position_global_to_ground(const vector_2dd &global_pos) const {
		const auto relative_pos = global_pos - transform2d().position();
		return vector_2dd(-std::atan2(relative_pos.y, relative_pos.x), relative_pos.length());
	}

	vector_2dd c_planet_ground_2d::convert_position_ground_to_global(const vector_2dd &ground_pos) const {
		return vector_2dd(std::cos(-ground_pos.x), std::sin(-ground_pos.x)) * ground_pos.y + transform2d().position();
	}

	vector_2dd c_planet_ground_2d::normalize_ground_vector_with_ground_position(const vector_2dd &ground_pos,
	                                                                         const vector_2dd &ground_vector) const {
		return vector_2dd(ground_pos.y == 0 ? 0 : ground_vector.x / ground_pos.y, ground_vector.y);
	}

	double c_planet_ground_2d::get_angle_difference_in_rad_ground_from_global_with_ground_position(
		const vector_2dd &ground_pos) const { return ground_pos.x + math::pi / 2; }
}
