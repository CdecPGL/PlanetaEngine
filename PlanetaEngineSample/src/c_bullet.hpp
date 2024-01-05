#pragma once

#include "planeta/planeta_engine.hpp"

class c_bullet final : public plnt::game_object_standard_component {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_bullet);

public:
	using super = game_object_standard_component;
	std::string erase_effect_go_id() const;
	void erase_effect_go_id(const std::string& id);
	void set_shooter(const plnt::weak_pointer<plnt::i_game_object> &shooter);

private:
	std::string erase_effect_go_id_;
	plnt::weak_pointer<plnt::i_game_object> shooter_;

protected:
	void on_initialized() override;
};

PE_GAMEOBJECTCOMPONENT_CLASS(c_bullet);
