#include "CEnemy.hpp"
#include "CDamageReceiver.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(CEnemy) {
	registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(CEnemy, hp)
		.PE_REFLECTABLE_CLASS_PROPERTY(CEnemy, defeated_effect_resource_id)
		.ShallowCopyTarget(&CEnemy::hp_)
		.ShallowCopyTarget(&CEnemy::defeated_effect_resource_id_);
}

int CEnemy::hp() const {
	return hp_;
}

void CEnemy::hp(int v) {
	hp_ = v;
}

std::string CEnemy::defeated_effect_resource_id() const {
	return defeated_effect_resource_id_;
}

void CEnemy::defeated_effect_resource_id(const std::string& id) {
	defeated_effect_resource_id_ = id;
}

void CEnemy::OnInitialized() {
	Super::OnInitialized();
	game_object().GetComponent<CDamageReceiver>()->damage_received.connect(CDamageReceiver::DamageReceivedEventType::slot_type([this](const EADamageReceived& e){
		hp_ -= e.damage;
		if (hp_ <= 0) {
			//やられたときエフェクトを生成する
			auto my_t = game_object().GetComponent<CTransform2D>();
			auto eff = scene().game_object_manager().CreateGameObjectWithComponentTypeIDList({ "Transform2D", "Effect","DisposerWhenEffectStop" });
			auto ceff = eff->GetComponent<CEffect>();
			ceff->resource_id(defeated_effect_resource_id_);
			ceff->roop_flag(false);
			auto ef_t = eff->GetComponent<CTransform2D>();
			ef_t->position(my_t->position());
			ef_t->scale({ 20,20 });
			eff->Activate();
			//自分を破棄
			game_object().Dispose();
		}
	}).track_foreign(shared_from_this()));
}
