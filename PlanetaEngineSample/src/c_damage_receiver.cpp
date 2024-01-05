#include "c_damage_receiver.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_damage_receiver) {
	registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(c_damage_receiver, is_damage_receivable)
		.shallow_copy_target(&c_damage_receiver::is_damage_receivable_);
}

bool c_damage_receiver::is_damage_receivable() const { return is_damage_receivable_; }

void c_damage_receiver::is_damage_receivable(const bool f) { is_damage_receivable_ = f; }

void c_damage_receiver::add_damage(const int damage, const int invincible_frame) {
	//無敵時間が0だったらダメージを受けつける
	if (invincible_frame_counter_ <= 0 && is_damage_receivable_) {
		invincible_frame_counter_ = invincible_frame;
		//イベント発生
		damage_received({damage});
	}
}

void c_damage_receiver::on_initialized() {
	super::on_initialized();
	//無敵時間カウントを減らすタスクを設定する
	const auto task = game_object().create_and_attach_task<t_instant>(task_slot::pre_collision_early_phase);
	task->set_execute_function([this] {
		if (invincible_frame_counter_ > 0) {
			--invincible_frame_counter_;
			//無敵時間終了時はイベントを発生させる
			if (invincible_frame_counter_ == 0) { invincible_time_ended(); }
		}
	});
}
