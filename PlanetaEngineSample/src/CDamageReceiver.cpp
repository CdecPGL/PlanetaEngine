#include "CDamageReceiver.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(CDamageReceiver) {
	registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(CDamageReceiver, is_damage_receivable)
		.ShallowCopyTarget(&CDamageReceiver::is_damage_receivable_);
}

bool CDamageReceiver::is_damage_receivable() const {
	return is_damage_receivable_;
}

void CDamageReceiver::is_damage_receivable(bool f) {
	is_damage_receivable_ = f;
}

void CDamageReceiver::AddDamage(int damage, int invincible_frame) {
	//���G���Ԃ�0��������_���[�W���󂯂���
	if (invincible_frame_counter_ <= 0 && is_damage_receivable_) {
		invincible_frame_counter_ = invincible_frame;
		//�C�x���g����
		damage_received({ damage });
	}
}

void CDamageReceiver::OnInitialized() {
	Super::OnInitialized();
	//���G���ԃJ�E���g�����炷�^�X�N��ݒ肷��
	auto task = game_object().CreateAndAttachTask<TInstant>(TaskSlot::PreCollisionEarlyPhase);
	task->SetExcuteFunction([this] {
		if (invincible_frame_counter_ > 0) { 
			--invincible_frame_counter_; 
			//���G���ԏI�����̓C�x���g�𔭐�������
			if (invincible_frame_counter_ == 0) {
				invincible_time_ended();
			}
		}
	});
}

