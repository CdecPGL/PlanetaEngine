#include "c_enemy.hpp"
#include "c_damage_receiver.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_enemy) {
	registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(c_enemy, hp)
		.PE_REFLECTABLE_CLASS_PROPERTY(c_enemy, defeated_effect_resource_id)
		.shallow_copy_target(&c_enemy::hp_)
		.shallow_copy_target(&c_enemy::defeated_effect_resource_id_);
}

int c_enemy::hp() const { return hp_; }

void c_enemy::hp(const int v) { hp_ = v; }

std::string c_enemy::defeated_effect_resource_id() const { return defeated_effect_resource_id_; }

void c_enemy::defeated_effect_resource_id(const std::string &id) { defeated_effect_resource_id_ = id; }

void c_enemy::on_initialized() {
	super::on_initialized();
	game_object().get_component<c_damage_receiver>()->damage_received.connect(
		c_damage_receiver::damage_received_event_type::slot_type([this](const e_damage_received &e) {
			hp_ -= e.damage;
			if (hp_ <= 0) {
				//やられたときエフェクトを生成する
				const auto my_t = game_object().get_component<c_transform_2d>();
				const auto eff = scene().game_object_manager().create_game_object_with_component_type_id_list({
					"transform_2d", "effect", "disposer_when_effect_stop"
				});
				const auto c_eff = eff->get_component<c_effect>();
				c_eff->resource_id(defeated_effect_resource_id_);
				c_eff->loop_flag(false);
				const auto ef_t = eff->get_component<c_transform_2d>();
				ef_t->position(my_t->position());
				ef_t->scale({20, 20});
				eff->activate();
				//自分を破棄
				game_object().dispose();
			}
		}).track_foreign(shared_from_this()));
}
