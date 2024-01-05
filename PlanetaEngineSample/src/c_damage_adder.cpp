#pragma warning(disable:4996)

#include "c_damage_adder.hpp"
#include "c_damage_receiver.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_damage_adder) {
	registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(c_damage_adder, power)
		.PE_REFLECTABLE_CLASS_PROPERTY(c_damage_adder, add_invincible_frame)
		.shallow_copy_target(&c_damage_adder::power_)
		.shallow_copy_target(&c_damage_adder::add_invincible_frame_);
}

bool c_damage_adder::get_other_components_proc(const go_component_getter &com_getter) {
	if (!super::get_other_components_proc(com_getter)) { return false; }
	//"DamageAdder"を持つコライダーに衝突イベントハンドラを登録する。
	for (const auto dmg_adder_coms = com_getter.get_all_components_with_condition<c_collider_2d>(
		     [](const game_object_component &g_com) { return g_com.labels().contains("damage_adder"); }); auto &&com :
	     dmg_adder_coms) {
		com->collided_with_collider2d.connect_member_function(
			std::static_pointer_cast<c_damage_adder>(shared_from_this()),
			&c_damage_adder::collision_handler);
	}
	return true;
}

void c_damage_adder::collision_handler(const plnt::e_collision_with_collider_2d &e) {
	//"DamageReceiver"ラベルを持つコライダーで、そのゲームオブジェクトがCDamageReceiverを持っている場合ダメージを与える。
	if (e.collider.labels().contains("damage_receiver")) {
		if (const auto dmg_receiver = e.collider.game_object().get_component<c_damage_receiver>()) {
			//ダメージを与える
			dmg_receiver->add_damage(power_, add_invincible_frame_);
			//イベント発生
			damage_added({});
		}
	}
}

int c_damage_adder::power() const { return power_; }

void c_damage_adder::power(const int p) { power_ = p; }

int c_damage_adder::add_invincible_frame() const { return add_invincible_frame_; }

void c_damage_adder::add_invincible_frame(const int inv_f) { add_invincible_frame_ = inv_f; }
