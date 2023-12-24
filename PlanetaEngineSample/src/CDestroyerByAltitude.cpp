#include "CDestroyerByAltitude.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(CDestroyerByAltitude) {
	registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(CDestroyerByAltitude, altitude_limit)
		.ShallowCopyTarget(&CDestroyerByAltitude::altitude_limit_);
}

bool CDestroyerByAltitude::GetOtherComponentsProc(const GOComponentGetter& com_getter) {
	if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
	my_c_transform2d_.reset(com_getter.GetComponent<CTransform2D>());
	return true;
}

void CDestroyerByAltitude::OnInitialized() {
	Super::OnInitialized();
	auto t = game_object().CreateAndAttachTask<TInstant>(TaskSlot::PreCollisionEarlyPhase);
	t->SetExcuteFunction([this] {
		auto g = my_c_transform2d_->GetGround();
		if (g->GetAltitudeWithGroundPosition(my_c_transform2d_->ground_position()) > altitude_limit()) {
			game_object().Dispose();
		}
	});
}
