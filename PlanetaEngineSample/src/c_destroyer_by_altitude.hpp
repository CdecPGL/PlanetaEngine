#pragma once

#include "planeta/planeta_engine.hpp"

class c_destroyer_by_altitude final : public plnt::game_object_standard_component {
public:
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_destroyer_by_altitude);
	using super = game_object_standard_component;
	[[nodiscard]] double altitude_limit() const { return altitude_limit_; }
	void altitude_limit(const double l) { altitude_limit_ = l; }

private:
	double altitude_limit_ = 0.0;
	plnt::weak_pointer<plnt::c_transform_2d> my_c_transform2d_;
	[[nodiscard]] bool get_other_components_proc(const plnt::go_component_getter &) override;
	void on_initialized() override;
};

PE_GAMEOBJECTCOMPONENT_CLASS(c_destroyer_by_altitude);
