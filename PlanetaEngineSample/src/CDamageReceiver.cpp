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
	//無敵時間が0だったらダメージを受けつける
	if (invincible_frame_counter_ <= 0 && is_damage_receivable_) {
		invincible_frame_counter_ = invincible_frame;
		//イベント発生
		damage_received({ damage });
	}
}

void CDamageReceiver::OnInitialized() {
	Super::OnInitialized();
	//無敵時間カウントを減らすタスクを設定する
	auto task = game_object().CreateAndAttachTask<TInstant>(TaskSlot::PreCollisionEarlyPhase);
	task->SetExcuteFunction([this] {
		if (invincible_frame_counter_ > 0) { 
			--invincible_frame_counter_; 
			//無敵時間終了時はイベントを発生させる
			if (invincible_frame_counter_ == 0) {
				invincible_time_ended();
			}
		}
	});
}

