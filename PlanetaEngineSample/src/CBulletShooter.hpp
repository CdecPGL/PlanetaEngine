#pragma once

#include "planeta/PlanetaEngine.hpp"

class CBulletShooter : public plnt::GameObjectStandardComponent {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(CBulletShooter);
public:
	using Super = plnt::GameObjectStandardComponent;

	std::vector<plnt::WeakPointer<plnt::IGameObject>> Shoot(int setting_idx, double dir_rad, plnt::CoordinateSystem shoot_space);

	struct ShootSetting : public plnt::reflection::Reflectable {
		PE_REFLECTION_DATA_REGISTERER_DECLARATION(ShootSetting);
		int interval;
		std::vector<std::string> gameobject_def_file_id_list;
		std::vector<double> speed_list;
		std::vector<double> default_dir_deg_list;
		std::vector<plnt::Vector2Dd> relative_position_list;
	};
	void shoot_settings(const std::vector<ShootSetting>& d);
protected:
	virtual void OnInitialized();
private:
	std::vector<ShootSetting> shoot_settings_;
	int count_since_last_shot_ = 0;
	virtual std::vector<plnt::WeakPointer<plnt::IGameObject>> ShootProc(const ShootSetting& shoot_setting, double dir_rad, plnt::CoordinateSystem shoot_space) = 0;
};
PE_GAMEOBJECTCOMPONENT_CLASS(CBulletShooter);
PE_REFLECTABLE_CLASS_WITH_ID(CBulletShooter::ShootSetting,CBulletShooter_ShootSetting);

