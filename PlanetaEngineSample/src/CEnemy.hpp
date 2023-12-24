#pragma once

#include "planeta/PlanetaEngine.hpp"

class CEnemy : public plnt::GameObjectStandardComponent {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(CEnemy);
public:
	using Super = plnt::GameObjectStandardComponent;
	int hp()const;
	void hp(int v);
	std::string defeated_effect_resource_id()const;
	void defeated_effect_resource_id(const std::string& id);
	using Super = plnt::GameObjectStandardComponent;
private:
	int hp_;
	std::string defeated_effect_resource_id_;
protected:
	virtual void OnInitialized() override;
};
PE_GAMEOBJECTCOMPONENT_CLASS(CEnemy);