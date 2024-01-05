#pragma once

#include "c_ground_2d.hpp"

namespace plnt {
	/*! 何もしない地面コンポーネント*/
	class c_dummy_ground_2d final : public c_ground_2d {
	public:
		using super = c_ground_2d;

	private:
		double get_altitude_with_ground_position([[maybe_unused]] const vector_2dd &ground_pos) const override { return -1; }
		vector_2dd convert_position_global_to_ground(const vector_2dd &global_pos) const override { return global_pos; }
		vector_2dd convert_position_ground_to_global(const vector_2dd &ground_pos) const override { return ground_pos; }

		vector_2dd normalize_ground_vector_with_ground_position([[maybe_unused]] const vector_2dd &ground_pos,
		                                                  const vector_2dd &ground_vector) const override {
			return ground_vector;
		}

		double get_angle_difference_in_rad_ground_from_global_with_ground_position([[maybe_unused]] const vector_2dd &ground_pos) const override {
			return 0;
		}
	};

	//PE_GAMEOBJECTCOMPONENT_CLASS(CDummyGround2D);
}
