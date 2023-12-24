#pragma warning(disable:4996)

#include "CDamageAdder.hpp"
#include "CDamageReceiver.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(CDamageAdder) {
	registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(CDamageAdder, power)
		.PE_REFLECTABLE_CLASS_PROPERTY(CDamageAdder, add_invincible_frame)
		.ShallowCopyTarget(&CDamageAdder::power_)
		.ShallowCopyTarget(&CDamageAdder::add_invincible_frame_);
}

bool CDamageAdder::GetOtherComponentsProc(const GOComponentGetter& com_getter) {
	if (!Super::GetOtherComponentsProc(com_getter)) { return false; }
	//"DamageAdder"�����R���C�_�[�ɏՓ˃C�x���g�n���h����o�^����B
	auto dmg_adde_coms = com_getter.GetAllComponentsWithCondition<CCollider2D>([](const GameObjectComponent& gcom) {
		return gcom.labels().count("DamageAdder") > 0;
	});
	for (auto&& com : dmg_adde_coms) {
		com->collided_with_collider2d.ConnectMemberFunction(std::static_pointer_cast<CDamageAdder>(shared_from_this()), &CDamageAdder::CollisionHandler);
	}
	return true;
}

void CDamageAdder::CollisionHandler(const plnt::EACollisionWithCollider2D& e) {
	//"DamageReciever"���x�������R���C�_�[�ŁA���̃Q�[���I�u�W�F�N�g��CDamagReceiver�������Ă���ꍇ�_���[�W��^����B
	if (e.collider.labels().count("DamageReceiver") > 0) {
		auto dmg_rcvr = e.collider.game_object().GetComponent<CDamageReceiver>();
		if (dmg_rcvr) {
			//�_���[�W��^����
			dmg_rcvr->AddDamage(power_, add_invincible_frame_);
			//�C�x���g����
			damage_added({});
		}
	}
}

int CDamageAdder::power() const {
	return power_;
}

void CDamageAdder::power(int p) {
	power_ = p;
}

int CDamageAdder::add_invincible_frame() const {
	return add_invincible_frame_;
}

void CDamageAdder::add_invincible_frame(int inv_f) {
	add_invincible_frame_ = inv_f;
}
