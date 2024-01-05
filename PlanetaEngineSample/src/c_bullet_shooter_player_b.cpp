#include "planeta/math.hpp"

#include "c_bullet_shooter_player_b.hpp"

using namespace plnt;

std::vector<weak_pointer<i_game_object>> c_bullet_shooter_player::shoot_proc(
	const shoot_setting &shoot_setting, const double dir_rad, const coordinate_system shoot_space) {
	std::vector<weak_pointer<i_game_object>> ret{};
	const auto my_trans = game_object().get_component<c_transform_2d>();
	for (size_t idx = 0; idx < shoot_setting.default_dir_deg_list.size(); ++idx) {
		const auto dir_rad_div = math::deg_to_rad(shoot_setting.default_dir_deg_list[idx]);
		//指定ゲームオブジェクトが複数ある場合は、順番に発射していく。
		const auto go_def_idx = (shoot_counter_++) % shoot_setting.game_object_def_file_id_list.size();
		auto go = scene().game_object_manager().create_game_object(
			shoot_setting.game_object_def_file_id_list[go_def_idx]);

		const auto bl_trans = go->get_component<c_transform_2d>();
		bl_trans->set_ground(my_trans->get_ground()->game_object().get_pointer(), false);
		bl_trans->velocity_space(shoot_space);

		vector_2dd gap{};
		if (idx < shoot_setting.relative_position_list.size()) { gap = shoot_setting.relative_position_list[idx]; }
		bl_trans->ground_position(my_trans->ground_position());
		bl_trans->ground_move(gap);
		switch (shoot_space) {
			case coordinate_system::global:
				bl_trans->rotation_rad(dir_rad_div + dir_rad);
				bl_trans->velocity(
					get_unit_vector_by_radian<double>(dir_rad_div + dir_rad) * shoot_setting.speed_list[0]);
				break;
			case coordinate_system::ground:
				bl_trans->ground_rotation_rad(dir_rad_div + dir_rad);
				bl_trans->ground_velocity(
					get_unit_vector_by_radian<double>(dir_rad_div + dir_rad) * shoot_setting.speed_list[0]);
				break;
		}
		go->activate();
		ret.push_back(go);
	}

	return ret;
}
