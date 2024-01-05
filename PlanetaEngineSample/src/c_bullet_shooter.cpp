#include "c_bullet_shooter.hpp"
#include "c_bullet.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_bullet_shooter) {
	registerer
		.write_only_property("shoot_settings", &c_bullet_shooter::shoot_settings)
		.shallow_copy_target(&c_bullet_shooter::shoot_settings_);
}

PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_bullet_shooter::shoot_setting) {
	registerer
		.PE_REFLECTABLE_CLASS_VARIABLE(shoot_setting, interval)
		.PE_REFLECTABLE_CLASS_VARIABLE(shoot_setting, game_object_def_file_id_list)
		.PE_REFLECTABLE_CLASS_VARIABLE(shoot_setting, speed_list)
		.PE_REFLECTABLE_CLASS_VARIABLE(shoot_setting, default_dir_deg_list)
		.PE_REFLECTABLE_CLASS_VARIABLE(shoot_setting, relative_position_list);
}

std::vector<weak_pointer<i_game_object>> c_bullet_shooter::shoot(const int setting_idx, const double dir_rad,
                                                               const coordinate_system shoot_space) {
	//弾設定のインデックスが正常か確認
	if (setting_idx < 0 || static_cast<unsigned>(setting_idx) >= shoot_settings_.size()) { return {}; }
	const auto &setting = shoot_settings_[setting_idx];
	//弾の発射間隔を確認
	if (count_since_last_shot_ > 0) { return {}; }
	count_since_last_shot_ = setting.interval;
	return shoot_proc(setting, dir_rad, shoot_space);
}

void c_bullet_shooter::shoot_settings(const std::vector<shoot_setting> &d) { shoot_settings_ = d; }

void c_bullet_shooter::on_initialized() {
	super::on_initialized();
	const auto tsk = game_object().create_and_attach_task<t_instant>(task_slot::pre_collision_early_phase);
	tsk->set_execute_function([this]() { if (count_since_last_shot_ > 0) { --count_since_last_shot_; } });
}
