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
				return std::string{".squat"};
			}
			return std::string{".ground"};
		}
		return std::string{".jump"};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_state() & player_input_code::shot_attack) {
			return std::string{"shot"};
		}
		if (go.my_c_player_input_preserver().input_state() & player_input_code::up) { return std::string{"lookup"}; }
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
			if (input_dir > 0 && g_vel.x < 0) { return std::string{".turn"}; }
			return std::string{".run"};
		}
		if (std::fabs(g_vel.x) < DBL_EPSILON) { return std::string{".idol"}; }
		return std::string{".brake"};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_down() & player_input_code::jump) { return std::string{"jump"}; }
		if (go.my_c_player_input_preserver().input_state() & player_input_code::down) { return std::string{"squat"}; }
		return {};
	}

	boost::optional<std::string> message_handler(c_player &, const std::string &) override { return {}; }
};

//--待機
class ngi_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("idol", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
			go.is_reverse(true);
			return std::string{"run"};
		}
		if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
			go.is_reverse(false);
			return std::string{"run"};
		}
		return {};
	}
};

//--走る
class ngr_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("run", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.is_reverse()) {
			//左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
				//右が押されたら
				return std::string{"turn"};
			}
			if ((go.my_c_player_input_preserver().input_state() & player_input_code::left) !=
				player_input_code::left) {
				//左が離されたら 
				return std::string{"brake"};
			}
		} else {
			//右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
				//左が押されたら
				return std::string{"turn"};
			}
			if ((go.my_c_player_input_preserver().input_state() & player_input_code::right) !=
				player_input_code::right) {
				//右が離されたら
				return std::string{"brake"};
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
			return std::string{"idol"};
		}
		go.is_reverse(vel.x < 0); //速度の方向に向きを設定
		go.my_c_graph_animator().start_animation("brake", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.is_reverse()) {
			//左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
				return std::string{"turn"}; //右が押されたら
			}
			if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
				//左が押されたら
				return std::string{"run"};
			}
		} else {
			//右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
				//左が押されたら
				return std::string{"turn"};
			}
			if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
				//右が押されたら
				return std::string{"run"};
			}
		}

		double decelerate = go.friction();
		if (auto vel = go.my_c_transform2d().ground_velocity(); std::abs(vel.x) < decelerate) {
			go.my_c_transform2d().ground_velocity({0, vel.y});
			return std::string{"idol"};
		}
		go.my_c_transform2d().ground_accelerate({go.is_reverse() ? decelerate : -decelerate, 0});
		return {};
	}
};

//--切り替えし
class ngt_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("turn", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.is_reverse()) {
			//左に進行している場合
			if ((go.my_c_player_input_preserver().input_state() & player_input_code::right) !=
				player_input_code::right) {
				//右が離されたら
				return std::string{"brake"};
			}
			if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
				//左が押されたら
				return std::string{"run"};
			}
		} else {
			//右に進行している場合
			if ((go.my_c_player_input_preserver().input_state() & player_input_code::left) != player_input_code::left) {
				//左が離されたら
				return std::string{"brake"};
			}
			if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
				//右が押されたら
				return std::string{"run"};
			}
		}

		double decelerate = go.friction() + go.run_acceleration() / 2;
		if (auto vel = go.my_c_transform2d().ground_velocity(); std::abs(vel.x) < decelerate) {
			go.my_c_transform2d().ground_velocity({0, vel.y});
			go.is_reverse(!go.is_reverse());
			return std::string{"run"};
		}
		go.my_c_transform2d().ground_accelerate({go.is_reverse() ? decelerate : -decelerate, 0});
		return {};
	}
};

//-ジャンプ
class nj_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("jump", false);
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
		if (mess == "collided_with_ground_enter") { return std::string{"ground"}; }
		return {};
	}
};

//-しゃがみ
class ns_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("squat", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		//しゃがみ解除
		if (go.my_c_player_input_preserver().input_up() & player_input_code::down) { return std::string{"ground"}; }
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
		if (go.my_c_circle_collider2d().is_grounded()) { return std::string{".ground"}; }
		return std::string{".jump"};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_state() & player_input_code::shot_attack) {
			return std::string{"shot"};
		}
		if ((go.my_c_player_input_preserver().input_state() & player_input_code::up) != player_input_code::up) {
			return std::string{"normal"};
		}
		return {};
	}
};

//-地上
class lg_state final : public c_state_machine::state<c_player> {
	boost::optional<std::string> enter_proc(c_player &go) override {
		//子ステートの振り分け
		if (go.my_c_player_input_preserver().input_state() & (player_input_code::left | player_input_code::right)) {
			return std::string{".run"};
		}
		return std::string{".idol"};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_down() & player_input_code::jump) { return std::string{"jump"}; }
		return {};
	}
};

//--待機
class lgi_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("lookup", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		go.my_c_transform2d().ground_velocity({0, go.my_c_transform2d().ground_velocity().y});
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
			go.is_reverse(true);
			return std::string{"run"};
		}
		if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
			go.is_reverse(false);
			return std::string{"run"};
		}
		return {};
	}
};

//--走る
class lgr_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("lookup_walk", true);
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
				return std::string{"idol"};
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
				return std::string{"idol"};
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
		go.my_c_graph_animator().start_animation("lookup_jump", false);
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
		if (mess == "collided_with_ground_enter") { return std::string{"ground"}; }
		return {};
	}
};

//ショット
class s_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		//子ステートの振り分け
		if (go.my_c_circle_collider2d().is_grounded()) { return std::string{".ground"}; }
		return std::string{".jump"};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if ((go.my_c_player_input_preserver().input_state() & player_input_code::shot_attack) !=
			player_input_code::shot_attack) {
			if (go.my_c_player_input_preserver().input_state() & player_input_code::up) {
				return std::string{"lookup"};
			}
			return std::string{"normal"};
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
			return std::string{".run"};
		}
		return std::string{".idol"};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_down() & player_input_code::jump) { return std::string{"jump"}; }
		return {};
	}
};

//--待機
class sgi_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("shot", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		go.my_c_transform2d().ground_velocity({0, go.my_c_transform2d().ground_velocity().y});
		return {};
	}

	boost::optional<std::string> update_proc(c_player &go) override {
		if (go.my_c_player_input_preserver().input_state() & player_input_code::left) {
			go.is_reverse(true);
			return std::string{"run"};
		}
		if (go.my_c_player_input_preserver().input_state() & player_input_code::right) {
			go.is_reverse(false);
			return std::string{"run"};
		}
		return {};
	}
};

//--走る
class sgr_state final : public c_state_machine::state<c_player> {
public:
	boost::optional<std::string> enter_proc(c_player &go) override {
		go.my_c_graph_animator().start_animation("shot_walk", true);
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
				return std::string{"idol"};
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
				return std::string{"idol"};
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
		go.my_c_graph_animator().start_animation("shot_jump", false);
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
		if (mess == "collided_with_ground_enter") { return std::string{"ground"}; }
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
	sm.register_state<n_state>("normal", *this);
	sm.register_state<ng_state>("normal.ground", *this);
	sm.register_state<ngi_state>("normal.ground.idol", *this);
	sm.register_state<ngr_state>("normal.ground.run", *this);
	sm.register_state<ngb_state>("normal.ground.brake", *this);
	sm.register_state<ngt_state>("normal.ground.turn", *this);
	sm.register_state<nj_state>("normal.jump", *this);
	sm.register_state<ns_state>("normal.squat", *this);
	//--見上げ
	sm.register_state<l_state>("lookup", *this);
	sm.register_state<lg_state>("lookup.ground", *this);
	sm.register_state<lgi_state>("lookup.ground.idol", *this);
	sm.register_state<lgr_state>("lookup.ground.run", *this);
	sm.register_state<lj_state>("lookup.jump", *this);
	//--ショット
	sm.register_state<s_state>("shot", *this);
	sm.register_state<sg_state>("shot.ground", *this);
	sm.register_state<sgi_state>("shot.ground.idol", *this);
	sm.register_state<sgr_state>("shot.ground.run", *this);
	sm.register_state<sj_state>("shot.jump", *this);
	//防御
	sm.register_state<d_state>("defense", *this);

	//Normal.Ground.Idolステートを開始
	sm.start_state_machine("normal.ground.idol");
}

void c_player::on_finalized() noexcept {
	on_collided_with_ground_connection_.disconnect();
	super::on_finalized();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void c_player::on_collided_with_ground(const e_collision_with_ground_2d &e) {
	switch (e.collision_state) {
		case collision_state::enter:
			my_c_state_machine().send_message("collided_with_ground_enter");
			break;
		case collision_state::exit:
			my_c_state_machine().send_message("collided_with_ground_exit");
			break;
		case collision_state::stay:
			break;
	}
}
