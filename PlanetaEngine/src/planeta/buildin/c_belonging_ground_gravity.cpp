#include "c_belonging_ground_gravity.hpp"
#include "c_transform_2d.hpp"
#include "c_ground_2d.hpp"
#include "../core/i_game_object.hpp"
#include "t_instant.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_belonging_ground_gravity) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(c_belonging_ground_gravity, gravity_scale)
			.shallow_copy_target(&c_belonging_ground_gravity::gravity_scale_);
	}

	bool c_belonging_ground_gravity::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		transform2d_.reset(com_getter.get_component<c_transform_2d>());
		return true;
	}

	void c_belonging_ground_gravity::on_initialized() {
		super::on_initialized();
		const auto proc = game_object().create_and_attach_task<t_instant>(task_slot::pre_collision_early_phase);
		proc->set_execute_function([this]() { update(); });
	}

	void c_belonging_ground_gravity::update() const {
		transform2d_->ground_accelerate(vector_2dd(0.0f, -1.0f) * gravity_scale_);
	}
}
