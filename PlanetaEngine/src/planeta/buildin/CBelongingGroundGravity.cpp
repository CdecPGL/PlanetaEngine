#include "CBelongingGroundGravity.hpp"
#include "CTransform2D.hpp"
#include "CGround2D.hpp"
#include "..\core\i_game_object.hpp"

#include "TInstant.hpp"

namespace plnt {
	PE_REFLECTION_DATA_REGISTERER_DEFINITION(CBelongingGroundGravity) {
		registerer
			.PE_REFLECTABLE_CLASS_PROPERTY(CBelongingGroundGravity, gravity_scale)
			.shallow_copy_target(&CBelongingGroundGravity::_gravity_scale);
	}

	bool CBelongingGroundGravity::get_other_components_proc(const go_component_getter &com_getter) {
		if (!super::get_other_components_proc(com_getter)) { return false; }
		transform2d_.reset(com_getter.get_component<CTransform2D>());
		return true;
	}

	void CBelongingGroundGravity::on_initialized() {
		super::on_initialized();
		auto proc = game_object().create_and_attach_task<TInstant>(TaskSlot::PreCollisionEarlyPhase);
		proc->SetExcuteFunction([this]() { Update(); });
	}

	void CBelongingGroundGravity::Update() { transform2d_->GroundAccelerate(vector_2dd(0.0f, -1.0f) * _gravity_scale); }
}
