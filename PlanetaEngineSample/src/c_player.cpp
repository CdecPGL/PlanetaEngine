#include "c_player.hpp"

using namespace plnt;
using player_input_code = c_player_input_observer::input_code;

//////////////////////////////////////////////////////////////////////////
//ステート
//////////////////////////////////////////////////////////////////////////
//通常
class n_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		//子ステートの振り分け
		if (go.my_c_circle_collider2d().is_grounded()) {
			if (go.my_c_player_input_preserver().input_state() & player_input_code::down) {
				return std::string{".Squat"};
			}
			return std::string{".Ground"};
		}
		return std::string{".Jump"};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_state() & player_input_code::shot_attack) {
			return std::string{"Shot"};
		}
		if (go.my_c_player_input_preserver().input_state() & player_input_code::up) { return std::string{"LookUp"}; }
		return {};
	}
};

//-地上
class ng_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		//子ステートの振り分け
		const auto &g_vel = go.my_c_transform2d().ground_velocity();
		int input_dir = 0; //入力方向(-1:左,0:なし,1:右)
		if (go.my_c_player_input_preserver().input_state() & player_input_code::left) { --input_dir; }
		if (go.my_c_player_input_preserver().input_state() & player_input_code::right) { ++input_dir; }
		if (input_dir) {
			//入力方向と速度の方向が反対だったら切り替えし動作
			if (input_dir > 0 && g_vel.x < 0) { return std::string{".Turn"}; }
			return std::string{".Run"};
		}
		if (std::fabs(g_vel.x) < DBL_EPSILON) { return std::string{".Idol"}; }
		return std::string{".Brake"};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_down() & player_input_code::jump) { return std::string{"Jump"}; }
		if (go.my_c_player_input_preserver().input_state() & player_input_code::down) { return std::string{"Squat"}; }
		return {};
	}

	boost::optional<std::string> message_handler(c_player &, const std::string &) override { return {}; }
};

//--待機
class ngi_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("Idol", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
			go.is_reverse(true);
			return std::string{"Run"};
		}
		if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
			go.is_reverse(false);
			return std::string{"Run"};
		}
		return {};
	}
};

//--走る
class ngr_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("Run", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.is_reverse()) {
			//左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
				//右が押されたら
				return std::string{"Turn"};
			}
			if ((go.my_c_player_input_preserver().input_state() & player_input_code::left) !=
				player_input_code::left) {
				//左が離されたら 
				return std::string{"Brake"};
			}
		} else {
			//右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
				//左が押されたら
				return std::string{"Turn"};
			}
			if ((go.my_c_player_input_preserver().input_state() & player_input_code::right) !=
				player_input_code::right) {
				//右が離されたら
				return std::string{"Brake"};
			}
		}

		double accelerate = go.run_acceleration();
		double max_spd = go.run_speed();
		go.my_c_transform2d().ground_accelerate({go.is_reverse() ? -accelerate : accelerate, 0});
		if (auto vel = go.my_c_transform2d().ground_velocity(); std::abs(vel.x) > max_spd) {
			go.my_c_transform2d().ground_velocity({go.is_reverse() ? -max_spd : max_spd, vel.y});
		}
		return {};
	}
};

//--ブレーキ
class ngb_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		const double decelerate = go.friction();
		auto vel = go.my_c_transform2d().ground_velocity();
		if (std::abs(vel.x) < decelerate) {
			go.my_c_transform2d().ground_velocity({0, vel.y});
			return std::string{"Idol"};
		}
		go.is_reverse(vel.x < 0); //速度の方向に向きを設定
		go.my_c_graph_animator().start_animation("Brake", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.is_reverse()) {
			//左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
				return std::string{"Turn"}; //右が押されたら
			}
			if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
				//左が押されたら
				return std::string{"Run"};
			}
		} else {
			//右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
				//左が押されたら
				return std::string{"Turn"};
			}
			if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
				//右が押されたら
				return std::string{"Run"};
			}
		}

		double decelerate = go.friction();
		if (auto vel = go.my_c_transform2d().ground_velocity(); std::abs(vel.x) < decelerate) {
			go.my_c_transform2d().ground_velocity({0, vel.y});
			return std::string{"Idol"};
		}
		go.my_c_transform2d().ground_accelerate({go.is_reverse() ? decelerate : -decelerate, 0});
		return {};
	}
};

//--切り替えし
class ngt_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("Turn", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.is_reverse()) {
			//左に進行している場合
			if ((go.my_c_player_input_preserver().input_state() & player_input_code::right) !=
				player_input_code::right) {
				//右が離されたら
				return std::string{"Brake"};
			}
			if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
				//左が押されたら
				return std::string{"Run"};
			}
		} else {
			//右に進行している場合
			if ((go.my_c_player_input_preserver().input_state() & player_input_code::left) != player_input_code::left) {
				//左が離されたら
				return std::string{"Brake"};
			}
			if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
				//右が押されたら
				return std::string{"Run"};
			}
		}

		double decelerate = go.friction() + go.run_acceleration() / 2;
		if (auto vel = go.my_c_transform2d().ground_velocity(); std::abs(vel.x) < decelerate) {
			go.my_c_transform2d().ground_velocity({0, vel.y});
			go.is_reverse(!go.is_reverse());
			return std::string{"Run"};
		}
		go.my_c_transform2d().ground_accelerate({go.is_reverse() ? decelerate : -decelerate, 0});
		return {};
	}
};

//-ジャンプ
class nj_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("Jump", false);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		if (go.my_c_circle_collider2d().is_grounded()) {
			go.my_c_transform2d().ground_accelerate({0, go.jump_power()});
		}
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		//向き変更
		if (go.my_c_player_input_preserver().input_down() & player_input_code::right) {
			go.is_reverse(false);
			go.my_c_draw_graph2d().reverse(false);
		}
		if (go.my_c_player_input_preserver().input_down() & player_input_code::left) {
			go.is_reverse(true);
			go.my_c_draw_graph2d().reverse(true);
		}
		//加速
		double accelerate = go.run_acceleration() / 2;
		double max_spd = go.run_speed();
		if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
			go.my_c_transform2d().ground_accelerate({accelerate, 0});
		} else if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
			go.my_c_transform2d().ground_accelerate({-accelerate, 0});
		}
		if (auto vel = go.my_c_transform2d().ground_velocity(); std::abs(vel.x) > max_spd) {
			go.my_c_transform2d().ground_velocity({go.is_reverse() ? -max_spd : max_spd, vel.y});
		}
		return {};
	}

	boost::optional<std::string> message_handler(c_player &, const std::string &mess) override {
		if (mess == "CollidedWithGroundEnter") { return std::string{"Ground"}; }
		return {};
	}
};

//-しゃがみ
class ns_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("Squat", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		//しゃがみ解除
		if (go.my_c_player_input_preserver().input_up() & player_input_code::down) { return std::string{"Ground"}; }
		//向き変更
		if (go.my_c_player_input_preserver().input_down() & player_input_code::right) {
			go.is_reverse(false);
			go.my_c_draw_graph2d().reverse(false);
		}
		if (go.my_c_player_input_preserver().input_down() & player_input_code::left) {
			go.is_reverse(true);
			go.my_c_draw_graph2d().reverse(true);
		}

		if (go.my_c_circle_collider2d().is_grounded()) {
			//しゃがみジャンプ
			if (go.my_c_player_input_preserver().input_down() & player_input_code::jump) {
				go.my_c_transform2d().ground_accelerate({0, 3});
			}
			//減速
			double decelerate = go.friction();
			if (auto vel = go.my_c_transform2d().ground_velocity(); std::abs(vel.x) < decelerate) {
				go.my_c_transform2d().ground_velocity({0, vel.y});
			} else { go.my_c_transform2d().ground_accelerate({vel.x < 0 ? decelerate : -decelerate, 0}); }
		}
		return {};
	}
};

//見上げ
class l_state final : public c_state_machine::state<c_player> {
	boost::optional<std::string> enter_proc(c_player &go) override {
		//子ステートの振り分け
		if (go.my_c_circle_collider2d().is_grounded()) { return std::string{".Ground"}; }
		return std::string{".Jump"};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_state() & player_input_code::shot_attack) {
			return std::string{"Shot"};
		}
		if ((go.my_c_player_input_preserver().input_state() & player_input_code::up) != player_input_code::up) {
			return std::string{"Normal"};
		}
		return {};
	}
};

//-地上
class lg_state final : public c_state_machine::state<c_player> {
	boost::optional<std::string> enter_proc(c_player &go) override {
		//子ステートの振り分け
		if (go.my_c_player_input_preserver().input_state() & (player_input_code::left | player_input_code::right)) {
			return std::string{".Run"};
		}
		return std::string{".Idol"};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_down() & player_input_code::jump) { return std::string{"Jump"}; }
		return {};
	}
};

//--待機
class lgi_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("LookUp", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		go.my_c_transform2d().ground_velocity({0, go.my_c_transform2d().ground_velocity().y});
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
			go.is_reverse(true);
			return std::string{"Run"};
		}
		if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
			go.is_reverse(false);
			return std::string{"Run"};
		}
		return {};
	}
};

//--走る
class lgr_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("LookUpWalk", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.is_reverse()) {
			//左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
				//右が押されたら
				go.is_reverse(false);
				go.my_c_draw_graph2d().reverse(false);
			} else if ((go.my_c_player_input_preserver().input_state() & player_input_code::left) !=
				player_input_code::left) {
				//左が離されたら 
				return std::string{"Idol"};
			}
		} else {
			//右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
				//左が押されたら
				go.is_reverse(true);
				go.my_c_draw_graph2d().reverse(true);
			} else if ((go.my_c_player_input_preserver().input_state() & player_input_code::right) !=
				player_input_code::right) {
				//右が離されたら
				return std::string{"Idol"};
			}
		}
		//左右移動
		int dir_var = 0;
		if (go.my_c_player_input_preserver().input_state() & player_input_code::right) { ++dir_var; }
		if (go.my_c_player_input_preserver().input_state() & player_input_code::left) { --dir_var; }
		const double spd = go.look_up_walk_speed();
		double vel = spd * dir_var;
		go.my_c_transform2d().ground_velocity({vel, go.my_c_transform2d().ground_velocity().y});
		return {};
	}
};

//-ジャンプ
class lj_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("LookUpJump", false);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		if (go.my_c_circle_collider2d().is_grounded()) {
			go.my_c_transform2d().ground_accelerate({0, go.look_up_jump_power()});
		}
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.is_reverse()) {
			//左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
				//右が押されたら
				go.is_reverse(false);
				go.my_c_draw_graph2d().reverse(false);
			}
		} else {
			//右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
				//左が押されたら
				go.is_reverse(true);
				go.my_c_draw_graph2d().reverse(true);
			}
		}
		//左右移動
		int dir_var = 0;
		if (go.my_c_player_input_preserver().input_state() & player_input_code::right) { ++dir_var; }
		if (go.my_c_player_input_preserver().input_state() & player_input_code::left) { --dir_var; }
		const double spd = go.look_up_walk_speed();
		double vel = spd * dir_var;
		go.my_c_transform2d().ground_velocity({vel, go.my_c_transform2d().ground_velocity().y});
		return {};
	}

	boost::optional<std::string> message_handler(c_player &, const std::string &mess) override {
		if (mess == "CollidedWithGroundEnter") { return std::string{"Ground"}; }
		return {};
	}
};

//ショット
class s_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		//子ステートの振り分け
		if (go.my_c_circle_collider2d().is_grounded()) { return std::string{".Ground"}; }
		return std::string{".Jump"};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if ((go.my_c_player_input_preserver().input_state() & player_input_code::shot_attack) !=
			player_input_code::shot_attack) {
			if (go.my_c_player_input_preserver().input_state() & player_input_code::up) {
				return std::string{"LookUp"};
			}
			return std::string{"Normal"};
		}
		//弾発射
		const auto ptr_scr_pos = go.my_c_player_input_preserver().pointer_position();
		const auto ptr_go_pos = go.game_object().scene().draw_system().
		                           covert_position_screen_space_to_game_object_space(ptr_scr_pos);
		const auto dir_vec = ptr_go_pos - go.my_c_transform2d().position();
		go.my_c_bullet_shooter().shoot(go.shoot_level(), std::atan2(dir_vec.y, dir_vec.x), coordinate_system::global);
		return {};
	}
};

//-地上
class sg_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		//子ステートの振り分け
		if (go.my_c_player_input_preserver().input_state() & (player_input_code::left | player_input_code::right)) {
			return std::string{".Run"};
		}
		return std::string{".Idol"};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_down() & player_input_code::jump) { return std::string{"Jump"}; }
		return {};
	}
};

//--待機
class sgi_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("Shot", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		go.my_c_transform2d().ground_velocity({0, go.my_c_transform2d().ground_velocity().y});
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
			go.is_reverse(true);
			return std::string{"Run"};
		}
		if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
			go.is_reverse(false);
			return std::string{"Run"};
		}
		return {};
	}
};

//--走る
class sgr_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("ShotWalk", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.is_reverse()) {
			//左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
				//右が押されたら
				go.is_reverse(false);
				go.my_c_draw_graph2d().reverse(false);
			} else if ((go.my_c_player_input_preserver().input_state() & player_input_code::left) !=
				player_input_code::left) {
				//左が離されたら 
				return std::string{"Idol"};
			}
		} else {
			//右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
				//左が押されたら
				go.is_reverse(true);
				go.my_c_draw_graph2d().reverse(true);
			} else if ((go.my_c_player_input_preserver().input_state() & player_input_code::right) !=
				player_input_code::right) {
				//右が離されたら
				return std::string{"Idol"};
			}
		}
		//左右移動
		int dir_var = 0;
		if (go.my_c_player_input_preserver().input_state() & player_input_code::right) { ++dir_var; }
		if (go.my_c_player_input_preserver().input_state() & player_input_code::left) { --dir_var; }
		const double spd = go.look_up_walk_speed();
		double vel = spd * dir_var;
		go.my_c_transform2d().ground_velocity({vel, go.my_c_transform2d().ground_velocity().y});
		return {};
	}
};

//-ジャンプ
class sj_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("ShotJump", false);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		if (go.my_c_circle_collider2d().is_grounded()) {
			go.my_c_transform2d().ground_accelerate({0, go.look_up_jump_power()});
		}
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.is_reverse()) {
			//左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
				//右が押されたら
				go.is_reverse(false);
				go.my_c_draw_graph2d().reverse(false);
			}
		} else {
			//右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
				//左が押されたら
				go.is_reverse(true);
				go.my_c_draw_graph2d().reverse(true);
			}
		}
		//左右移動
		int dir_var = 0;
		if (go.my_c_player_input_preserver().input_state() & player_input_code::right) { ++dir_var; }
		if (go.my_c_player_input_preserver().input_state() & player_input_code::left) { --dir_var; }
		const double spd = go.look_up_walk_speed();
		double vel = spd * dir_var;
		go.my_c_transform2d().ground_velocity({vel, go.my_c_transform2d().ground_velocity().y});
		return {};
	}

	boost::optional<std::string> message_handler(c_player &, const std::string &mess) override {
		if (mess == "CollidedWithGroundEnter") { return std::string{"Ground"}; }
		return {};
	}
};

//防御
class d_state final : public c_state_machine::state<c_player> {
};

//////////////////////////////////////////////////////////////////////////
//CPlayer
//////////////////////////////////////////////////////////////////////////

PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_player) {
	registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(c_player, run_speed)
		.PE_REFLECTABLE_CLASS_PROPERTY(c_player, friction)
		.PE_REFLECTABLE_CLASS_PROPERTY(c_player, run_acceleration)
		.PE_REFLECTABLE_CLASS_PROPERTY(c_player, jump_power)
		.PE_REFLECTABLE_CLASS_PROPERTY(c_player, jump_duration)
		.PE_REFLECTABLE_CLASS_PROPERTY(c_player, look_up_walk_speed)
		.PE_REFLECTABLE_CLASS_PROPERTY(c_player, look_up_jump_power)
		.PE_REFLECTABLE_CLASS_PROPERTY(c_player, shoot_level)
		.shallow_copy_target(&c_player::run_speed_)
		.shallow_copy_target(&c_player::friction_)
		.shallow_copy_target(&c_player::run_acceleration_)
		.shallow_copy_target(&c_player::jump_power_)
		.shallow_copy_target(&c_player::jump_duration_)
		.shallow_copy_target(&c_player::look_up_walk_speed_)
		.shallow_copy_target(&c_player::look_up_jump_power_)
		.shallow_copy_target(&c_player::shoot_level_);
}

bool c_player::get_other_components_proc(const go_component_getter &com_getter) {
	if (!super::get_other_components_proc(com_getter)) { return false; }
	my_c_transform2d_ = com_getter.get_component<c_transform_2d>();
	my_c_draw_graph2d_ = com_getter.get_component<c_draw_graph_2d>();
	my_c_graph_animator_ = com_getter.get_component<c_graph_2d_animator>();
	my_c_state_machine_ = com_getter.get_component<c_state_machine>();
	my_c_belonging_ground_gravity_ = com_getter.get_component<c_belonging_ground_gravity>();
	my_c_circle_collider2d_ = com_getter.get_component<c_circle_collider_2d>();
	on_collided_with_ground_connection_ = my_c_circle_collider2d_->collided_with_ground2d.connect_member_function(
		std::static_pointer_cast<c_player>(shared_from_this()), &c_player::on_collided_with_ground);
	my_c_bullet_shooter_ = com_getter.get_component<c_bullet_shooter>();
	my_c_player_input_preserver_ = com_getter.get_component<c_player_input_observer>();
	return true;
}

void c_player::on_initialized() {
	super::on_initialized();
	//ステートマシンの設定
	auto &sm = my_c_state_machine();
	//--通常
	sm.register_state<n_state>("Normal", *this);
	sm.register_state<ng_state>("Normal.Ground", *this);
	sm.register_state<ngi_state>("Normal.Ground.Idol", *this);
	sm.register_state<ngr_state>("Normal.Ground.Run", *this);
	sm.register_state<ngb_state>("Normal.Ground.Brake", *this);
	sm.register_state<ngt_state>("Normal.Ground.Turn", *this);
	sm.register_state<nj_state>("Normal.Jump", *this);
	sm.register_state<ns_state>("Normal.Squat", *this);
	//--見上げ
	sm.register_state<l_state>("LookUp", *this);
	sm.register_state<lg_state>("LookUp.Ground", *this);
	sm.register_state<lgi_state>("LookUp.Ground.Idol", *this);
	sm.register_state<lgr_state>("LookUp.Ground.Run", *this);
	sm.register_state<lj_state>("LookUp.Jump", *this);
	//--ショット
	sm.register_state<s_state>("Shot", *this);
	sm.register_state<sg_state>("Shot.Ground", *this);
	sm.register_state<sgi_state>("Shot.Ground.Idol", *this);
	sm.register_state<sgr_state>("Shot.Ground.Run", *this);
	sm.register_state<sj_state>("Shot.Jump", *this);
	//防御
	sm.register_state<d_state>("Defense", *this);

	//Normal.Ground.Idolステートを開始
	sm.start_state_machine("Normal.Ground.Idol");
}

void c_player::on_finalized() noexcept {
	on_collided_with_ground_connection_.disconnect();
	super::on_finalized();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void c_player::on_collided_with_ground(const e_collision_with_ground_2d &e) {
	switch (e.collision_state) {
		case collision_state::enter:
			my_c_state_machine().send_message("CollidedWithGroundEnter");
			break;
		case collision_state::exit:
			my_c_state_machine().send_message("CollidedWithGroundExit");
			break;
		case collision_state::stay:
			break;
	}
}
