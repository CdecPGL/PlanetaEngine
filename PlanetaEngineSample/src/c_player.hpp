#pragma once

#include "planeta/planeta_engine.hpp"

#include "c_state_machine.hpp"
#include "c_bullet_shooter.hpp"
#include "c_player_input_observer.hpp"

class c_player final : public plnt::game_object_standard_component {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_player);

public:
	using super = game_object_standard_component;

	[[nodiscard]] plnt::c_transform_2d &my_c_transform2d() const { return *my_c_transform2d_; }
	[[nodiscard]] plnt::c_draw_graph_2d &my_c_draw_graph2d() const { return *my_c_draw_graph2d_; }
	[[nodiscard]] plnt::c_graph_2d_animator &my_c_graph_animator() const { return *my_c_graph_animator_; }

	[[nodiscard]] plnt::c_belonging_ground_gravity &my_c_belonging_ground_gravity() const {
		return *my_c_belonging_ground_gravity_;
	}

	[[nodiscard]] c_state_machine &my_c_state_machine() const { return *my_c_state_machine_; }
	[[nodiscard]] plnt::c_circle_collider_2d &my_c_circle_collider2d() const { return *my_c_circle_collider2d_; }
	[[nodiscard]] c_bullet_shooter &my_c_bullet_shooter() const { return *my_c_bullet_shooter_; }
	[[nodiscard]] c_player_input_observer &my_c_player_input_preserver() const { return *my_c_player_input_preserver_; }

	[[nodiscard]] bool is_reverse() const { return reverse_flag_; }
	void is_reverse(const bool f) { reverse_flag_ = f; }

	[[nodiscard]] double run_speed() const { return run_speed_; }
	void run_speed(const double ms) { run_speed_ = ms; }
	[[nodiscard]] double friction() const { return friction_; }
	void friction(const double f) { friction_ = f; }
	[[nodiscard]] double run_acceleration() const { return run_acceleration_; }
	void run_acceleration(const double a) { run_acceleration_ = a; }
	[[nodiscard]] double jump_power() const { return jump_power_; }
	void jump_power(const double p) { jump_power_ = p; }
	[[nodiscard]] double jump_duration() const { return jump_duration_; }
	void jump_duration(const double d) { jump_duration_ = d; }
	[[nodiscard]] double look_up_walk_speed() const { return look_up_walk_speed_; }
	void look_up_walk_speed(const double s) { look_up_walk_speed_ = s; }
	[[nodiscard]] double look_up_jump_power() const { return look_up_jump_power_; }
	void look_up_jump_power(const double s) { look_up_jump_power_ = s; }
	[[nodiscard]] int shoot_level() const { return shoot_level_; }
	void shoot_level(const int sl) { shoot_level_ = sl; }

protected:
	[[nodiscard]] bool get_other_components_proc(const plnt::go_component_getter &) override;
	void on_initialized() override;
	void on_finalized() noexcept override;

private:
	double run_speed_ = 0;
	double friction_ = 0;
	double run_acceleration_ = 0;
	double jump_power_ = 0;
	double jump_duration_ = 0;
	double look_up_walk_speed_ = 0;
	double look_up_jump_power_ = 0;
	int shoot_level_ = 0;

	plnt::weak_pointer<plnt::c_transform_2d> my_c_transform2d_;
	plnt::weak_pointer<plnt::c_draw_graph_2d> my_c_draw_graph2d_;
	plnt::weak_pointer<plnt::c_graph_2d_animator> my_c_graph_animator_;
	plnt::weak_pointer<plnt::c_belonging_ground_gravity> my_c_belonging_ground_gravity_;
	plnt::weak_pointer<c_state_machine> my_c_state_machine_;
	plnt::weak_pointer<plnt::c_circle_collider_2d> my_c_circle_collider2d_;
	plnt::weak_pointer<c_bullet_shooter> my_c_bullet_shooter_;
	plnt::weak_pointer<c_player_input_observer> my_c_player_input_preserver_;
	plnt::signal_connection on_collided_with_ground_connection_;
	bool reverse_flag_ = false; //反転フラグ(右側を正面方向とする)

	void on_collided_with_ground(const plnt::e_collision_with_ground_2d &e);
};

PE_GAMEOBJECTCOMPONENT_CLASS(c_player);
