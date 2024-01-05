#pragma once

#include "planeta/planeta_engine.hpp"

#include "c_bullet_shooter.hpp"

class c_bullet_shooter_player final : public c_bullet_shooter {
public:
	using super = c_bullet_shooter;

private:
	std::vector<plnt::weak_pointer<plnt::i_game_object>> shoot_proc(const shoot_setting &shoot_setting, double dir_rad,
	                                                            plnt::coordinate_system shoot_space) override;
	int shoot_counter_ = 0;
};

PE_GAMEOBJECTCOMPONENT_CLASS(c_bullet_shooter_player);
