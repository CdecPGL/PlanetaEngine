#pragma once

#include "planeta/planeta_engine.hpp"

class c_enemy final : public plnt::game_object_standard_component {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_enemy);

public:
	using super = game_object_standard_component;
	[[nodiscard]] int hp() const;
	void hp(int v);
	[[nodiscard]] std::string defeated_effect_resource_id() const;
	void defeated_effect_resource_id(const std::string &id);

private:
	int hp_ = 0;
	std::string defeated_effect_resource_id_;

protected:
	void on_initialized() override;
};

PE_GAMEOBJECTCOMPONENT_CLASS(c_enemy);
