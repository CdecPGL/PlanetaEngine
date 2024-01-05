#pragma once

#include "planeta/planeta_engine.hpp"

class c_bullet_shooter : public plnt::game_object_standard_component {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_bullet_shooter);

public:
	using super = game_object_standard_component;

	std::vector<plnt::weak_pointer<plnt::i_game_object>> shoot(int setting_idx, double dir_rad,
	                                                           plnt::coordinate_system shoot_space);

	struct shoot_setting final : reflectable {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(shoot_setting);
		int interval;
		std::vector<std::string> game_object_def_file_id_list;
		std::vector<double> speed_list;
		std::vector<double> default_dir_deg_list;
		std::vector<plnt::vector_2dd> relative_position_list;
	};

	void shoot_settings(const std::vector<shoot_setting> &d);

protected:
	void on_initialized() override;

private:
	std::vector<shoot_setting> shoot_settings_;
	int count_since_last_shot_ = 0;
	virtual std::vector<plnt::weak_pointer<plnt::i_game_object>> shoot_proc(const shoot_setting &shoot_setting,
	                                                                        double dir_rad,
	                                                                        plnt::coordinate_system shoot_space) = 0;
};

PE_GAMEOBJECTCOMPONENT_CLASS(c_bullet_shooter);

PE_REFLECTABLE_CLASS_WITH_ID(c_bullet_shooter::shoot_setting, c_bullet_shooter_shoot_setting);
