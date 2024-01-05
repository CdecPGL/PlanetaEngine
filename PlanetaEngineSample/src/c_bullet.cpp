#include "c_bullet.hpp"
#include "c_damage_adder.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_bullet) {
	registerer
		.PE_REFLECTABLE_CLASS_PROPERTY(c_bullet, erase_effect_go_id)
		.shallow_copy_target(&c_bullet::erase_effect_go_id_);
}

std::string c_bullet::erase_effect_go_id() const { return erase_effect_go_id_; }

void c_bullet::erase_effect_go_id(const std::string& id) { erase_effect_go_id_ = id; }

void c_bullet::set_shooter(const weak_pointer<i_game_object> &shooter) { shooter_ = shooter; }

void c_bullet::on_initialized() {
	super::on_initialized();
	game_object().get_component<c_damage_adder>()->damage_added.connect_function(
		shared_from_this(), [this](const e_damage_added &)-> void {
			//必要なら、消えるときのエフェクトを作成
			if (!erase_effect_go_id_.empty()) {
				const auto ef = scene().game_object_manager().create_game_object(erase_effect_go_id_);
				const auto ef_trans = ef->get_component<c_transform_2d>();
				const auto my_trans = game_object().get_component<c_transform_2d>();
				ef_trans->position(my_trans->position());
				ef_trans->rotation_rad(my_trans->rotation_rad());
				ef_trans->scale(my_trans->scale());
				ef->activate();
			}
			game_object().dispose();
		});
}
