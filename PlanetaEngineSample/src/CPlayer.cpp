#include "CPlayer.hpp"

using namespace plnt;
using PlayerInputCode = CPlayerInputPreserver::InputCode;

//////////////////////////////////////////////////////////////////////////
//ステート
//////////////////////////////////////////////////////////////////////////
//通常
class NState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		//子ステートの振り分け
		if (go.my_c_circle_collider2d().is_grounded()) {
			if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Down) {
				return std::string{ ".Squat" };
			} else {
				return std::string{ ".Ground" };
			}
		} else {
			return std::string{ ".Jump" };
		}
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::ShotAttack) {
			return std::string{ "Shot" };
		} else if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Up) {
			return std::string{ "LookUp" };
		}
		return{};
	}
};
//-地上
class NGState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		//子ステートの振り分け
		const auto& g_vel = go.my_c_transform2d().ground_velocity();
		int input_dir = 0; //入力方向(-1:左,0:なし,1:右)
		if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Left) { --input_dir; }
		if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Right) { ++input_dir; }
		if (input_dir) {
			//入力方向と速度の方向が反対だったら切り替えし動作
			if (input_dir > 0 && g_vel.x < 0) {
				return std::string{ ".Turn" };
			} else {
				return std::string{ ".Run" };
			}
		} else {
			if (std::fabs(g_vel.x) < DBL_EPSILON) {
				return std::string{ ".Idol" };
			} else {
				return std::string{ ".Brake" };
			}
		}
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.my_c_player_input_preserver().input_down()&PlayerInputCode::Jump) {
			return std::string{ "Jump" };
		} else if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Down) {
			return std::string{ "Squat" };
		}
		return{};
	}
	virtual boost::optional<std::string> MessageHandler(CPlayer&, const std::string&) override {
		return{};
	}
};
//--待機
class NGIState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		go.my_c_graph_animator().StartAnimation("Idol", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return{};
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Left)) {
			go.is_reverse(true);
			return std::string{ "Run" };
		} else if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Right)) {
			go.is_reverse(false);
			return std::string{ "Run" };
		}
		return{};
	}
};
//--走る
class NGRState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		go.my_c_graph_animator().StartAnimation("Run", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return{};
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.is_reverse()) { //左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Right)) { //右が押されたら
				return std::string{ "Turn" };
			} else if ((go.my_c_player_input_preserver().input_state() & PlayerInputCode::Left) != PlayerInputCode::Left) { //左が離されたら 
				return std::string{ "Brake" };
			}
		} else { //右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Left)) { //左が押されたら
				return std::string{ "Turn" };
			} else if ((go.my_c_player_input_preserver().input_state() & PlayerInputCode::Right) != PlayerInputCode::Right) { //右が離されたら
				return std::string{ "Brake" };
			}
		}

		double accelerate = go.run_acceleration();
		double max_spd = go.run_speed();
		go.my_c_transform2d().GroundAccelerate({ go.is_reverse() ? -accelerate : accelerate,0 });
		auto vel = go.my_c_transform2d().ground_velocity();
		if (std::abs(vel.x) > max_spd) {
			go.my_c_transform2d().ground_velocity({ go.is_reverse() ? -max_spd : max_spd,vel.y });
		}
		return{};
	}
};
//--ブレーキ
class NGBState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		double deaccelerate = go.friction();
		auto vel = go.my_c_transform2d().ground_velocity();
		if (std::abs(vel.x) < deaccelerate) {
			go.my_c_transform2d().ground_velocity({ 0,vel.y });
			return std::string{ "Idol" };
		}
		go.is_reverse(vel.x < 0); //速度の方向に向きを設定
		go.my_c_graph_animator().StartAnimation("Brake", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return{};
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.is_reverse()) { //左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Right)) {
				return std::string{ "Turn" }; //右が押されたら
			} else if (go.my_c_player_input_preserver().input_state() & PlayerInputCode::Left) { //左が押されたら
				return std::string{ "Run" };
			}
		} else { //右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Left)) { //左が押されたら
				return std::string{ "Turn" };
			} else if (go.my_c_player_input_preserver().input_state() & PlayerInputCode::Right) { //右が押されたら
				return std::string{ "Run" };
			}
		}

		double deaccelerate = go.friction();
		auto vel = go.my_c_transform2d().ground_velocity();
		if (std::abs(vel.x) < deaccelerate) {
			go.my_c_transform2d().ground_velocity({ 0,vel.y });
			return std::string{ "Idol" };
		} else {
			go.my_c_transform2d().GroundAccelerate({ go.is_reverse() ? deaccelerate : -deaccelerate,0 });
		}
		return{};
	}
};
//--切り替えし
class NGTState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		go.my_c_graph_animator().StartAnimation("Turn", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return{};
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.is_reverse()) { //左に進行している場合
			if ((go.my_c_player_input_preserver().input_state() & PlayerInputCode::Right) != PlayerInputCode::Right) { //右が離されたら
				return std::string{ "Brake" };
			} else if (go.my_c_player_input_preserver().input_state() & PlayerInputCode::Left) { //左が押されたら
				return std::string{ "Run" };
			}
		} else { //右に進行している場合
			if ((go.my_c_player_input_preserver().input_state() & PlayerInputCode::Left) != PlayerInputCode::Left) { //左が離されたら
				return std::string{ "Brake" };
			} else if (go.my_c_player_input_preserver().input_state() & PlayerInputCode::Right) { //右が押されたら
				return std::string{ "Run" };
			}
		}

		double deaccelerate = go.friction() + go.run_acceleration() / 2;
		auto vel = go.my_c_transform2d().ground_velocity();
		if (std::abs(vel.x) < deaccelerate) {
			go.my_c_transform2d().ground_velocity({ 0,vel.y });
			go.is_reverse(!go.is_reverse());
			return std::string{ "Run" };
		} else {
			go.my_c_transform2d().GroundAccelerate({ go.is_reverse() ? deaccelerate : -deaccelerate,0 });
		}
		return{};
	}
};
//-ジャンプ
class NJState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		go.my_c_graph_animator().StartAnimation("Jump", false);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		if (go.my_c_circle_collider2d().is_grounded()) {
			go.my_c_transform2d().GroundAccelerate({ 0,go.jump_power() });
		}
		return{};
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		//向き変更
		if (go.my_c_player_input_preserver().input_down() & PlayerInputCode::Right) {
			go.is_reverse(false);
			go.my_c_draw_graph2d().reverse(false);
		}
		if (go.my_c_player_input_preserver().input_down() & PlayerInputCode::Left) {
			go.is_reverse(true);
			go.my_c_draw_graph2d().reverse(true);
		}
		//加速
		double accelerate = go.run_acceleration() / 2;
		double max_spd = go.run_speed();
		if (go.my_c_player_input_preserver().input_state() & PlayerInputCode::Right) {
			go.my_c_transform2d().GroundAccelerate({ accelerate,0 });
		} else if (go.my_c_player_input_preserver().input_state() & PlayerInputCode::Left) {
			go.my_c_transform2d().GroundAccelerate({ -accelerate,0 });
		}
		auto vel = go.my_c_transform2d().ground_velocity();
		if (std::abs(vel.x) > max_spd) {
			go.my_c_transform2d().ground_velocity({ go.is_reverse() ? -max_spd : max_spd,vel.y });
		}
		return{};
	}
	virtual boost::optional<std::string> MessageHandler(CPlayer&, const std::string& mess) override {
		if (mess == "CollidedWithGroundEnter") {
			return std::string{ "Ground" };
		}
		return{};
	}
};
//-しゃがみ
class NSState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		go.my_c_graph_animator().StartAnimation("Squat", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return{};
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		//しゃがみ解除
		if (go.my_c_player_input_preserver().input_up() & PlayerInputCode::Down) {
			return std::string{ "Ground" };
		}
		//向き変更
		if (go.my_c_player_input_preserver().input_down() & PlayerInputCode::Right) {
			go.is_reverse(false);
			go.my_c_draw_graph2d().reverse(false);
		}
		if (go.my_c_player_input_preserver().input_down() & PlayerInputCode::Left) {
			go.is_reverse(true);
			go.my_c_draw_graph2d().reverse(true);
		}

		if (go.my_c_circle_collider2d().is_grounded()) {
			//しゃがみジャンプ
			if (go.my_c_player_input_preserver().input_down()&PlayerInputCode::Jump) {
				go.my_c_transform2d().GroundAccelerate({ 0,3 });
			}
			//減速
			double deaccelerate = go.friction();
			auto vel = go.my_c_transform2d().ground_velocity();
			if (std::abs(vel.x) < deaccelerate) {
				go.my_c_transform2d().ground_velocity({ 0,vel.y });
			} else {
				go.my_c_transform2d().GroundAccelerate({ vel.x < 0 ? deaccelerate : -deaccelerate,0 });
			}
		}
		return{};
	}
};

//見上げ
class LState : public CStateMachine::State<CPlayer> {
	virtual boost::optional<std::string> EnterProc(CPlayer& go) {
		//子ステートの振り分け
		if (go.my_c_circle_collider2d().is_grounded()) {
			return std::string{ ".Ground" };
		} else {
			return std::string{ ".Jump" };
		}
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::ShotAttack) {
			return std::string{ "Shot" };
		} else if ((go.my_c_player_input_preserver().input_state()&PlayerInputCode::Up) != PlayerInputCode::Up) {
			return std::string{ "Normal" };
		}
		return{};
	}
};
//-地上
class LGState : public CStateMachine::State<CPlayer> {
	virtual boost::optional<std::string> EnterProc(CPlayer& go) {
		//子ステートの振り分け
		if (go.my_c_player_input_preserver().input_state()&(PlayerInputCode::Left | PlayerInputCode::Right)) {
			return std::string{ ".Run" };
		} else {
			return std::string{ ".Idol" };
		}
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.my_c_player_input_preserver().input_down()&PlayerInputCode::Jump) {
			return std::string{ "Jump" };
		}
		return{};
	}
};
//--待機
class LGIState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		go.my_c_graph_animator().StartAnimation("LookUp", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		go.my_c_transform2d().ground_velocity({ 0,go.my_c_transform2d().ground_velocity().y });
		return{};
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Left)) {
			go.is_reverse(true);
			return std::string{ "Run" };
		} else if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Right)) {
			go.is_reverse(false);
			return std::string{ "Run" };
		}
		return{};
	}
};
//--走る
class LGRState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		go.my_c_graph_animator().StartAnimation("LookUpWalk", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return{};
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.is_reverse()) { //左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Right)) { //右が押されたら
				go.is_reverse(false);
				go.my_c_draw_graph2d().reverse(false);
			} else if ((go.my_c_player_input_preserver().input_state() & PlayerInputCode::Left) != PlayerInputCode::Left) { //左が離されたら 
				return std::string{ "Idol" };
			}
		} else { //右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Left)) { //左が押されたら
				go.is_reverse(true);
				go.my_c_draw_graph2d().reverse(true);
			} else if ((go.my_c_player_input_preserver().input_state() & PlayerInputCode::Right) != PlayerInputCode::Right) { //右が離されたら
				return std::string{ "Idol" };
			}
		}
		//左右移動
		int dir_var = 0;
		if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Right) {
			++dir_var;
		}
		if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Left) {
			--dir_var;
		}
		double spd = go.look_up_walk_speed();
		double vel = spd * dir_var;
		go.my_c_transform2d().ground_velocity({ vel,go.my_c_transform2d().ground_velocity().y });
		return{};
	}
};
//-ジャンプ
class LJState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		go.my_c_graph_animator().StartAnimation("LookUpJump", false);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		if (go.my_c_circle_collider2d().is_grounded()) {
			go.my_c_transform2d().GroundAccelerate({ 0,go.look_up_jump_power() });
		}
		return{};
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.is_reverse()) { //左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Right)) { //右が押されたら
				go.is_reverse(false);
				go.my_c_draw_graph2d().reverse(false);
			}
		} else { //右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Left)) { //左が押されたら
				go.is_reverse(true);
				go.my_c_draw_graph2d().reverse(true);
			}
		}
		//左右移動
		int dir_var = 0;
		if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Right) {
			++dir_var;
		}
		if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Left) {
			--dir_var;
		}
		double spd = go.look_up_walk_speed();
		double vel = spd * dir_var;
		go.my_c_transform2d().ground_velocity({ vel,go.my_c_transform2d().ground_velocity().y });
		return{};
	}
	virtual boost::optional<std::string> MessageHandler(CPlayer&, const std::string& mess) override {
		if (mess == "CollidedWithGroundEnter") {
			return std::string{ "Ground" };
		}
		return{};
	}
};

//ショット
class SState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) {
		//子ステートの振り分け
		if (go.my_c_circle_collider2d().is_grounded()) {
			return std::string{ ".Ground" };
		} else {
			return std::string{ ".Jump" };
		}
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if ((go.my_c_player_input_preserver().input_state()&PlayerInputCode::ShotAttack) != PlayerInputCode::ShotAttack) {
			if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Up) {
				return std::string{ "LookUp" };
			} else {
				return std::string{ "Normal" };
			}
		}
		//弾発射
		auto ptr_scr_pos = go.my_c_player_input_preserver().pointer_position();
		auto ptr_go_pos = go.game_object().scene().draw_system().CovertPositionScreenSpaceToGameObjectSpace(ptr_scr_pos);
		auto dir_vec = ptr_go_pos - go.my_c_transform2d().position();
		go.my_c_bullet_shooter().Shoot(go.shoot_level(), std::atan2(dir_vec.y, dir_vec.x), CoordinateSystem::Global);
		return{};
	}
};
//-地上
class SGState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) {
		//子ステートの振り分け
		if (go.my_c_player_input_preserver().input_state()&(PlayerInputCode::Left | PlayerInputCode::Right)) {
			return std::string{ ".Run" };
		} else {
			return std::string{ ".Idol" };
		}
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.my_c_player_input_preserver().input_down()&PlayerInputCode::Jump) {
			return std::string{ "Jump" };
		}
		return{};
	}
};
//--待機
class SGIState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		go.my_c_graph_animator().StartAnimation("Shot", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		go.my_c_transform2d().ground_velocity({ 0,go.my_c_transform2d().ground_velocity().y });
		return{};
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Left)) {
			go.is_reverse(true);
			return std::string{ "Run" };
		} else if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Right)) {
			go.is_reverse(false);
			return std::string{ "Run" };
		}
		return{};
	}
};
//--走る
class SGRState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		go.my_c_graph_animator().StartAnimation("ShotWalk", true);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		return{};
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.is_reverse()) { //左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Right)) { //右が押されたら
				go.is_reverse(false);
				go.my_c_draw_graph2d().reverse(false);
			} else if ((go.my_c_player_input_preserver().input_state() & PlayerInputCode::Left) != PlayerInputCode::Left) { //左が離されたら 
				return std::string{ "Idol" };
			}
		} else { //右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Left)) { //左が押されたら
				go.is_reverse(true);
				go.my_c_draw_graph2d().reverse(true);
			} else if ((go.my_c_player_input_preserver().input_state() & PlayerInputCode::Right) != PlayerInputCode::Right) { //右が離されたら
				return std::string{ "Idol" };
			}
		}
		//左右移動
		int dir_var = 0;
		if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Right) {
			++dir_var;
		}
		if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Left) {
			--dir_var;
		}
		double spd = go.look_up_walk_speed();
		double vel = spd * dir_var;
		go.my_c_transform2d().ground_velocity({ vel,go.my_c_transform2d().ground_velocity().y });
		return{};
	}
};
//-ジャンプ
class SJState : public CStateMachine::State<CPlayer> {
public:
	virtual boost::optional<std::string> EnterProc(CPlayer& go) override {
		go.my_c_graph_animator().StartAnimation("ShotJump", false);
		go.my_c_draw_graph2d().reverse(go.is_reverse());
		if (go.my_c_circle_collider2d().is_grounded()) {
			go.my_c_transform2d().GroundAccelerate({ 0,go.look_up_jump_power() });
		}
		return{};
	}
	virtual boost::optional<std::string> UpdateProc(CPlayer& go) override {
		if (go.is_reverse()) { //左に進行している場合
			if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Right)) { //右が押されたら
				go.is_reverse(false);
				go.my_c_draw_graph2d().reverse(false);
			}
		} else { //右に進行している場合
			if (go.my_c_player_input_preserver().input_state() & (PlayerInputCode::Left)) { //左が押されたら
				go.is_reverse(true);
				go.my_c_draw_graph2d().reverse(true);
			}
		}
		//左右移動
		int dir_var = 0;
		if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Right) {
			++dir_var;
		}
		if (go.my_c_player_input_preserver().input_state()&PlayerInputCode::Left) {
			--dir_var;
		}
		double spd = go.look_up_walk_speed();
		double vel = spd * dir_var;
		go.my_c_transform2d().ground_velocity({ vel,go.my_c_transform2d().ground_velocity().y });
		return{};
	}
	virtual boost::optional<std::string> MessageHandler(CPlayer&, const std::string& mess) override {
		if (mess == "CollidedWithGroundEnter") {
			return std::string{ "Ground" };
		}
		return{};
	}
};

//防御
class DState : public CStateMachine::State<CPlayer> {
public:

};

//////////////////////////////////////////////////////////////////////////
//CPlayer
//////////////////////////////////////////////////////////////////////////

PE_REFLECTION_DATA_REGISTERER_DEFINITION(CPlayer) {
	registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(CPlayer, run_speed)
		.PE_REFLECTABLE_CLASS_PROPERTY(CPlayer, friction)
		.PE_REFLECTABLE_CLASS_PROPERTY(CPlayer, run_acceleration)
		.PE_REFLECTABLE_CLASS_PROPERTY(CPlayer, jump_power)
		.PE_REFLECTABLE_CLASS_PROPERTY(CPlayer, jump_duration)
		.PE_REFLECTABLE_CLASS_PROPERTY(CPlayer, look_up_walk_speed)
		.PE_REFLECTABLE_CLASS_PROPERTY(CPlayer, look_up_jump_power)
		.PE_REFLECTABLE_CLASS_PROPERTY(CPlayer, shoot_level)
		.ShallowCopyTarget(&CPlayer::run_speed_)
		.ShallowCopyTarget(&CPlayer::friction_)
		.ShallowCopyTarget(&CPlayer::run_acceleration_)
		.ShallowCopyTarget(&CPlayer::jump_power_)
		.ShallowCopyTarget(&CPlayer::jump_duration_)
		.ShallowCopyTarget(&CPlayer::look_up_walk_speed_)
		.ShallowCopyTarget(&CPlayer::look_up_jump_power_)
		.ShallowCopyTarget(&CPlayer::shoot_level_);
}

bool CPlayer::GetOtherComponentsProc(const GOComponentGetter& com_getter) {
	if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
	my_c_transform2d_.reset(com_getter.GetComponent<CTransform2D>());
	my_c_draw_graph2d_.reset(com_getter.GetComponent<CDrawGraph2D>());
	my_c_graph_animator_.reset(com_getter.GetComponent<CGraph2DAnimator>());
	my_c_state_machine_.reset(com_getter.GetComponent<CStateMachine>());
	my_c_belonging_ground_gravity_.reset(com_getter.GetComponent<CBelongingGroundGravity>());
	my_c_circle_collider2d_.reset(com_getter.GetComponent<CCircleCollider2D>());
	on_collided_with_ground_connection_ = my_c_circle_collider2d_->collided_with_ground2d.ConnectMemberFunction(std::static_pointer_cast<CPlayer>(shared_from_this()), &CPlayer::OnCollidedWithGround);
	my_c_bullet_shooter_.reset(com_getter.GetComponent<CBulletShooter>());
	my_c_player_input_preserver_.reset(com_getter.GetComponent<CPlayerInputPreserver>());
	return true;
}

void CPlayer::OnInitialized() {
	Super::OnInitialized();
	//ステートマシンの設定
	auto& sm = my_c_state_machine();
	//--通常
	sm.RegisterState<NState>("Normal", *this);
	sm.RegisterState<NGState>("Normal.Ground", *this);
	sm.RegisterState<NGIState>("Normal.Ground.Idol", *this);
	sm.RegisterState<NGRState>("Normal.Ground.Run", *this);
	sm.RegisterState<NGBState>("Normal.Ground.Brake", *this);
	sm.RegisterState<NGTState>("Normal.Ground.Turn", *this);
	sm.RegisterState<NJState>("Normal.Jump", *this);
	sm.RegisterState<NSState>("Normal.Squat", *this);
	//--見上げ
	sm.RegisterState<LState>("LookUp", *this);
	sm.RegisterState<LGState>("LookUp.Ground", *this);
	sm.RegisterState<LGIState>("LookUp.Ground.Idol", *this);
	sm.RegisterState<LGRState>("LookUp.Ground.Run", *this);
	sm.RegisterState<LJState>("LookUp.Jump", *this);
	//--ショット
	sm.RegisterState<SState>("Shot", *this);
	sm.RegisterState<SGState>("Shot.Ground", *this);
	sm.RegisterState<SGIState>("Shot.Ground.Idol", *this);
	sm.RegisterState<SGRState>("Shot.Ground.Run", *this);
	sm.RegisterState<SJState>("Shot.Jump", *this);
	//防御
	sm.RegisterState<DState>("Defense", *this);

	//Normal.Ground.Idolステートを開始
	sm.StartStateMachine("Normal.Ground.Idol");
}

void CPlayer::OnFinalized() noexcept {
	on_collided_with_ground_connection_.Disconnect();
	Super::OnFinalized();
}

void CPlayer::OnCollidedWithGround(const plnt::EACollisionWithGround2D& e) {
	switch (e.collision_state) {
	case plnt::CollisionState::Enter:
		my_c_state_machine().SendMessage("CollidedWithGroundEnter");
		break;
	case plnt::CollisionState::Exit:
		my_c_state_machine().SendMessage("CollidedWithGroundExit");
		break;
	default:
		break;
	}
}
