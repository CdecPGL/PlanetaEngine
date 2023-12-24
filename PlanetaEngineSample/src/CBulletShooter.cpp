#include "CBulletShooter.hpp"
#include "CBullet.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(CBulletShooter) {
	registerer
		.WriteOnlyProperty("shoot_settings",&CBulletShooter::shoot_settings)
		.ShallowCopyTarget(&CBulletShooter::shoot_settings_);
}

PE_REFLECTION_DATA_REGISTERER_DEFINITION(CBulletShooter::ShootSetting) {
	registerer
		.PE_REFLECTABLE_CLASS_VARIABLE(ShootSetting, interval)
		.PE_REFLECTABLE_CLASS_VARIABLE(ShootSetting, gameobject_def_file_id_list)
		.PE_REFLECTABLE_CLASS_VARIABLE(ShootSetting, speed_list)
		.PE_REFLECTABLE_CLASS_VARIABLE(ShootSetting, default_dir_deg_list)
		.PE_REFLECTABLE_CLASS_VARIABLE(ShootSetting, relative_position_list);
}

std::vector<plnt::WeakPointer<plnt::IGameObject>> CBulletShooter::Shoot(int setting_idx, double dir_rad, plnt::CoordinateSystem shoot_space) {
	//弾設定のインデックスが正常か確認
	if (setting_idx < 0 || (unsigned)setting_idx >= shoot_settings_.size()) { return{}; }
	const auto& setting = shoot_settings_[setting_idx];
	//弾の発射間隔を確認
	if (count_since_last_shot_ > 0) { return{}; }
	count_since_last_shot_ = setting.interval;
	return std::move(ShootProc(setting, dir_rad, shoot_space));
}

void CBulletShooter::shoot_settings(const std::vector<ShootSetting>& d) {
	shoot_settings_ = d;
}

void CBulletShooter::OnInitialized() {
	Super::OnInitialized();
	auto tsk = game_object().CreateAndAttachTask<TInstant>(TaskSlot::PreCollisionEarlyPhase);
	tsk->SetExcuteFunction([this]() {
		if (count_since_last_shot_ > 0) { --count_since_last_shot_; }
	});
}
