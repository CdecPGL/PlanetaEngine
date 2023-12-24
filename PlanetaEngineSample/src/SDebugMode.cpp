#include "SDebugMode.hpp"

#include "TPlayerInputSender.hpp"
#include "TCameraFollower.hpp"

using namespace plnt;

bool SDebugMode::SetUpScene(SceneSetUpProxy& scene_proxy, const util::ParameterHolder&) {
	auto& scene = scene_proxy.scene();
	auto camera = scene.game_object_manager().CreateGameObject("Camera");
	camera->Activate();
	auto planet = scene.game_object_manager().CreateGameObject("Planet0");
	planet->Activate();
	auto g2p = planet->GetComponent<CTransform2D>();
	g2p->position({ 0,0 });
	auto player = scene.game_object_manager().CreateGameObject("PlayerB");
	player->Activate();
	auto p2p = player->GetComponent<CTransform2D>();
	p2p->SetGround(planet, false);
	p2p->position({ 100,100 });
	p2p->ground_rotation_rad(0);
	auto t_pic = scene.task_manager().CreateAndRunTask<TPlayerInputSender>(TaskSlot::PlayerInputUpdatePhase);
	t_pic->SetTarget(player); 
	auto t_cf = scene.task_manager().CreateAndRunTask<TCameraFollower>(TaskSlot::CameraUpdatePhase);
	t_cf->SetCamera(camera);
	t_cf->SetTarget(player, false);

	constexpr int hakokun_count = 20;
	for (int i = 0; i < hakokun_count; ++i) {
		auto hakokun = scene.game_object_manager().CreateGameObject(i>0 ? "Hakokun" : "Hakosama");
		auto h_trans = hakokun->GetComponent<CTransform2D>();
		h_trans->SetGround(planet, false);
		h_trans->ground_position({ 6.283/hakokun_count*i,400 });
		h_trans->ground_rotation_rad(0);
		hakokun->Activate();
	}

	return true;
}

util::ParameterHolder SDebugMode::FinalizeScene(SceneSetUpProxy&, const std::string&, const util::ParameterHolder&) {
	return{};
}

