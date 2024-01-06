#include "s_debug_mode.hpp"
#include "t_player_input_sender.hpp"
#include "t_camera_follower.hpp"

using namespace plnt;

bool s_debug_mode::setup_scene(scene_setup_proxy &scene_proxy, const util::parameter_holder &) {
	auto &scene = scene_proxy.scene();
	const auto camera = scene.game_object_manager().create_game_object("camera");
	camera->activate();
	const auto planet = scene.game_object_manager().create_game_object("planet");
	planet->activate();
	const auto g2_p = planet->get_component<c_transform_2d>();
	g2_p->position({0, 0});
	const auto player = scene.game_object_manager().create_game_object("player");
	player->activate();
	const auto p2_p = player->get_component<c_transform_2d>();
	p2_p->set_ground(planet, false);
	p2_p->position({100, 100});
	p2_p->ground_rotation_rad(0);
	const auto t_pic = scene.task_manager().create_and_run_task<t_player_input_sender>(task_slot::player_input_update_phase);
	t_pic->set_target(player);
	const auto t_cf = scene.task_manager().create_and_run_task<t_camera_follower>(task_slot::camera_update_phase);
	t_cf->set_camera(camera);
	t_cf->set_target(player, false);

	constexpr int hakokun_count = 20;
	for (int i = 0; i < hakokun_count; ++i) {
		const auto hakokun = scene.game_object_manager().create_game_object(i > 0 ? "hakokun" : "hakosama");
		const auto h_trans = hakokun->get_component<c_transform_2d>();
		h_trans->set_ground(planet, false);
		h_trans->ground_position({6.283 / hakokun_count * i, 400});
		h_trans->ground_rotation_rad(0);
		hakokun->activate();
	}

	return true;
}

util::parameter_holder s_debug_mode::finalize_scene(scene_setup_proxy &, const std::string &, const util::parameter_holder &) {
	return {};
}
