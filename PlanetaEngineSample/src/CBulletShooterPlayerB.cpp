#include "planeta/Math.hpp"

#include "CBulletShooterPlayerB.hpp"

using namespace plnt;

CBulletShooterPlayer::CBulletShooterPlayer() {}


CBulletShooterPlayer::~CBulletShooterPlayer() {}

std::vector<plnt::WeakPointer<plnt::IGameObject>> CBulletShooterPlayer::ShootProc(const ShootSetting& shoot_setting, double dir_rad, plnt::CoordinateSystem shoot_space) {
	std::vector<plnt::WeakPointer<plnt::IGameObject>> ret{};
	auto my_trans = game_object().GetComponent<CTransform2D>();
	for (size_t idx = 0; idx < shoot_setting.default_dir_deg_list.size(); ++idx) {
		auto dir_rad_div = math::DegToRad(shoot_setting.default_dir_deg_list[idx]);
		//指定ゲームオブジェクトが複数ある場合は、順番に発射していく。
		auto go_def_idx = (shoot_counter_++) % shoot_setting.gameobject_def_file_id_list.size();
		auto go = scene().game_object_manager().CreateGameObject(shoot_setting.gameobject_def_file_id_list[go_def_idx]);

		auto bl_trans = go->GetComponent<CTransform2D>();
		bl_trans->SetGround(my_trans->GetGround()->game_object().GetPointer(), false);
		bl_trans->velocity_space(shoot_space);

		Vector2Dd gap{};
		if (idx < shoot_setting.relative_position_list.size()) {
			gap = shoot_setting.relative_position_list[idx];
		}
		bl_trans->ground_position(my_trans->ground_position());
		bl_trans->GroundMove(gap);
		switch (shoot_space) {
		case plnt::CoordinateSystem::Global:
			bl_trans->rotation_rad(dir_rad_div + dir_rad);
			bl_trans->velocity(GetUnitVectorByRadian<double>(dir_rad_div + dir_rad)*shoot_setting.speed_list[0]);
			break;
		case plnt::CoordinateSystem::Ground:
			bl_trans->ground_rotation_rad(dir_rad_div + dir_rad);
			bl_trans->ground_velocity(GetUnitVectorByRadian<double>(dir_rad_div + dir_rad)*shoot_setting.speed_list[0]);
			break;
		default:
			assert(false);
			break;
		}
		go->Activate();
		ret.push_back(go);
	}
	
	return std::move(ret);
}
