#pragma once

#include "planeta/PlanetaEngine.hpp"

#include "CBulletShooter.hpp"

class CBulletShooterPlayer final : public CBulletShooter {
public:
	using Super = CBulletShooter;
	CBulletShooterPlayer();
	~CBulletShooterPlayer();
private:
	std::vector<plnt::WeakPointer<plnt::IGameObject>> ShootProc(const ShootSetting& shoot_setting, double dir_rad, plnt::CoordinateSystem shoot_space) override;
	int shoot_counter_ = 0;
};
PE_GAMEOBJECTCOMPONENT_CLASS(CBulletShooterPlayer);
