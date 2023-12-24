#pragma once

#include "planeta/PlanetaEngine.hpp"

class CBullet : public plnt::GameObjectStandardComponent {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(CBullet);
public:
	using Super = plnt::GameObjectStandardComponent;
	std::string erase_effect_go_id()const;
	void erase_effect_go_id(const std::string id);
	void SetShooter(const plnt::WeakPointer<plnt::IGameObject>& shooter);
private:
	std::string erase_effect_go_id_;
	plnt::WeakPointer<plnt::IGameObject> shooter_;
protected:

	virtual void OnInitialized() override;

};
PE_GAMEOBJECTCOMPONENT_CLASS(CBullet);
