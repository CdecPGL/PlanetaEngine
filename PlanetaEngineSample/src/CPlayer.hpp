#pragma once

#include "planeta/PlanetaEngine.hpp"

#include "CStateMachine.hpp"
#include "CBulletShooter.hpp"
#include "CPlayerInputPreserver.hpp"

class CPlayer : public plnt::GameObjectStandardComponent {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(CPlayer);
public:
	using Super = plnt::GameObjectStandardComponent;

	plnt::CTransform2D& my_c_transform2d() { return *my_c_transform2d_; }
	plnt::CDrawGraph2D& my_c_draw_graph2d() { return *my_c_draw_graph2d_; }
	plnt::CGraph2DAnimator& my_c_graph_animator() { return *my_c_graph_animator_; }
	plnt::CBelongingGroundGravity& my_c_belonging_ground_gravity() { return *my_c_belonging_ground_gravity_; }
	CStateMachine& my_c_state_machine() { return *my_c_state_machine_; }
	plnt::CCircleCollider2D& my_c_circle_collider2d() { return *my_c_circle_collider2d_; }
	CBulletShooter& my_c_bullet_shooter() { return *my_c_bullet_shooter_; }
	CPlayerInputPreserver& my_c_player_input_preserver() { return *my_c_player_input_preserver_; }

	bool is_reverse() { return reverse_flag_; }
	void is_reverse(bool f) { reverse_flag_ = f; }

	double run_speed()const { return run_speed_; }
	void run_speed(double ms) { run_speed_ = ms; }
	double friction()const { return friction_; }
	void friction(double f) { friction_ = f; }
	double run_acceleration()const { return run_acceleration_; }
	void run_acceleration(double a) { run_acceleration_ = a; }
	double jump_power()const { return jump_power_; }
	void jump_power(double p) { jump_power_ = p; }
	double jump_duration()const { return jump_duration_; }
	void jump_duration(double d) { jump_duration_ = d; }
	double look_up_walk_speed()const { return look_up_walk_speed_; }
	void look_up_walk_speed(double s) { look_up_walk_speed_ = s; }
	double look_up_jump_power()const { return look_up_jump_power_; }
	void look_up_jump_power(double s) { look_up_jump_power_ = s; }
	int shoot_level()const { return shoot_level_; }
	void shoot_level(int sl) { shoot_level_ = sl; }
protected:
	virtual bool GetOtherComponentsProc(const plnt::GOComponentGetter&) override;
	virtual void OnInitialized() override;
	virtual void OnFinalized() noexcept override;
private:
	double run_speed_ = 0;
	double friction_ = 0;
	double run_acceleration_ = 0;
	double jump_power_ = 0;
	double jump_duration_ = 0;
	double look_up_walk_speed_ = 0;
	double look_up_jump_power_ = 0;
	int shoot_level_ = 0;

	plnt::NonOwingPointer<plnt::CTransform2D> my_c_transform2d_;
	plnt::NonOwingPointer<plnt::CDrawGraph2D> my_c_draw_graph2d_;
	plnt::NonOwingPointer<plnt::CGraph2DAnimator> my_c_graph_animator_;
	plnt::NonOwingPointer<plnt::CBelongingGroundGravity> my_c_belonging_ground_gravity_;
	plnt::NonOwingPointer<CStateMachine> my_c_state_machine_;
	plnt::NonOwingPointer<plnt::CCircleCollider2D> my_c_circle_collider2d_;
	plnt::NonOwingPointer<CBulletShooter> my_c_bullet_shooter_;
	plnt::NonOwingPointer<CPlayerInputPreserver> my_c_player_input_preserver_;
	plnt::SignalConnection on_collided_with_ground_connection_;
	bool reverse_flag_ = false; //îΩì]ÉtÉâÉO(âEë§Çê≥ñ ï˚å¸Ç∆Ç∑ÇÈ)

	void OnCollidedWithGround(const plnt::EACollisionWithGround2D& e);
};
PE_GAMEOBJECTCOMPONENT_CLASS(CPlayer);