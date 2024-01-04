#pragma once

#include "CGround2D.hpp"

namespace plnt {
	class c_planet;

	/*! 惑星地面コンポーネント*/
	class c_planet_ground_2d final : public c_ground_2d {
	public:
		using super = c_planet_ground_2d;

	private:
		non_owing_pointer<c_planet> planet_component_ = {};
		[[nodiscard]] bool get_other_components_proc(const go_component_getter &com_getter) override;
		bool collide_with(c_circle_collider_2d &circle_collider) override;
		[[nodiscard]] double get_altitude_with_ground_position(const vector_2dd &ground_pos) const override;
		[[nodiscard]] vector_2dd convert_position_global_to_ground(const vector_2dd &global_pos) const override;
		[[nodiscard]] vector_2dd convert_position_ground_to_global(const vector_2dd &ground_pos) const override;
		[[nodiscard]] vector_2dd normalize_ground_vector_with_ground_position(const vector_2dd &ground_pos,
		                                                  const vector_2dd &ground_vector) const override;
		[[nodiscard]] double get_angle_difference_in_rad_ground_from_global_with_ground_position(const vector_2dd &ground_pos) const override;
	};

	PE_GAMEOBJECTCOMPONENT_CLASS(c_planet_ground_2d);
}
