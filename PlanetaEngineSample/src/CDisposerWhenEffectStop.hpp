#pragma once

#include "planeta/PlanetaEngine.hpp"

class CDisposerWhenEffectStop final: public plnt::GameObjectStandardComponent {
public:
	using Super = plnt::GameObjectStandardComponent;
private:

protected:
	int frame_counter_ = 0;
	virtual void OnInitialized() override;

};
PE_GAMEOBJECTCOMPONENT_CLASS(CDisposerWhenEffectStop);
