#pragma once

#include "planeta/planeta_engine.hpp"

class c_disposer_when_effect_stop final : public plnt::game_object_standard_component {
public:
	using super = game_object_standard_component;

protected:
	int frame_counter_ = 0;
	void on_initialized() override;
};

PE_GAMEOBJECTCOMPONENT_CLASS(c_disposer_when_effect_stop);
