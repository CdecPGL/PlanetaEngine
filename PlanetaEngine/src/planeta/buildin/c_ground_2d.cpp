#include "../core/matrix_22.hpp"
#include "../core/i_game_object.hpp"
#include "../core/log_utility.hpp"
#include "c_ground_2d.hpp"
#include "c_transform_2d.hpp"

namespace plnt {
	c_ground_2d::c_ground_2d(): transform2d_() {}

	bool c_ground_2d::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		transform2d_.reset(com_getter.get_component<c_transform_2d>());
		if (!transform2d_) {
			PE_LOG_ERROR("Transform2Dが取得できませんでした。");
			return false;
		}
		return true;
	}

	void c_ground_2d::on_activated() { super::on_activated(); }

	void c_ground_2d::on_inactivated() { super::on_inactivated(); }

	void c_ground_2d::on_finalized() noexcept {}

	double c_ground_2d::convert_rotation_global_to_ground_with_ground_position(const vector_2dd &ground_pos,
	                                                                         double global_rota_rad) const {
		return global_rota_rad + get_angle_difference_in_rad_ground_from_global_with_ground_position(ground_pos);
	}

	double c_ground_2d::convert_rotation_ground_to_global_with_ground_position(const vector_2dd &ground_pos,
	                                                                         double ground_rota_rad) const {
		return ground_rota_rad - get_angle_difference_in_rad_ground_from_global_with_ground_position(ground_pos);
	}

	vector_2dd c_ground_2d::convert_velocity_global_to_ground_with_ground_position(
		const vector_2dd &ground_pos, const vector_2dd &global_velocity) const {
		return math::rotation_transform(get_angle_difference_in_rad_ground_from_global_with_ground_position(ground_pos),
		                                global_velocity);
	}

	vector_2dd c_ground_2d::convert_velocity_ground_to_global_with_ground_position(
		const vector_2dd &ground_pos, const vector_2dd &ground_velocity) const {
		return math::rotation_transform(
			-get_angle_difference_in_rad_ground_from_global_with_ground_position(ground_pos),
			ground_velocity);
	}

	c_transform_2d &c_ground_2d::transform2d() { return *transform2d_; }

	const plnt::c_transform_2d &c_ground_2d::transform2d() const { return *transform2d_; }
}
