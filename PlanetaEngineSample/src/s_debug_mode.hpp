#pragma once

#include "planeta/planeta_engine.hpp"

class s_debug_mode final : public plnt::scene_setupper {
public:
	using super = scene_setupper;
	bool setup_scene(plnt::scene_setup_proxy &scene_proxy,
	                 const plnt::util::parameter_holder &initialize_parameters) override;

	plnt::util::parameter_holder finalize_scene(plnt::scene_setup_proxy &scene_proxy, const std::string &next_scene_id,
	                                            const plnt::util::parameter_holder &finalize_parameters) override;
};

PE_SCENE_CLASS(s_debug_mode);
