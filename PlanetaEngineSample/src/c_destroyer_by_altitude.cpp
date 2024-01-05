#include "c_destroyer_by_altitude.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_destroyer_by_altitude) {
	registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(c_destroyer_by_altitude, altitude_limit)
		.shallow_copy_target(&c_destroyer_by_altitude::altitude_limit_);
}

bool c_destroyer_by_altitude::get_other_components_proc(const go_component_getter &com_getter) {
	if (!super::get_other_components_proc(com_getter)) { return false; }
	my_c_transform2d_ = com_getter.get_component<c_transform_2d>();
	return true;
}

void c_destroyer_by_altitude::on_initialized() {
	super::on_initialized();
	const auto t = game_object().create_and_attach_task<t_instant>(task_slot::pre_collision_early_phase);
	t->set_execute_function([this] {
		const auto g = my_c_transform2d_->get_ground();
		if (g->get_altitude_with_ground_position(my_c_transform2d_->ground_position()) > altitude_limit()) {
			game_object().dispose();
		}
	});
}
