#include "CBullet.hpp"
#include "CDamageAdder.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(CBullet) {
	registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(CBullet,erase_effect_go_id)
		.ShallowCopyTarget(&CBullet::erase_effect_go_id_);
}

std::string CBullet::erase_effect_go_id() const {
	return erase_effect_go_id_;
}

void CBullet::erase_effect_go_id(const std::string id) {
	erase_effect_go_id_ = id;
}

void CBullet::SetShooter(const plnt::WeakPointer<plnt::IGameObject>& shooter) {
	shooter_ = shooter;
}

void CBullet::OnInitialized() {
	Super::OnInitialized();
	game_object().GetComponent<CDamageAdder>()->damage_added.ConnectFunction(shared_from_this(), [this](const EADamageAdded&)->void {
		//必要なら、消えるときのエフェクトを作成
		if (!erase_effect_go_id_.empty()) {
			auto ef = scene().game_object_manager().CreateGameObject(erase_effect_go_id_);
			auto ef_trans = ef->GetComponent<CTransform2D>();
			auto my_trans = game_object().GetComponent<CTransform2D>();
			ef_trans->position(my_trans->position());
			ef_trans->rotation_rad(my_trans->rotation_rad());
			ef_trans->scale(my_trans->scale());
			ef->Activate();
		}
		game_object().Dispose();
	});
}
