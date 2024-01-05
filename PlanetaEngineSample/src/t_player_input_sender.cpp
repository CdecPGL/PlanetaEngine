#include "t_player_input_sender.hpp"
#include "c_player_input_observer.hpp"

using namespace plnt;

bool t_player_input_sender::set_target(const weak_pointer<i_game_object> &game_object) {
	if (const auto gi_pc = game_object->get_component<c_player_input_observer>()) {
		if (target_) { target_->reset_input(); }
		target_ = gi_pc;
		return true;
	}
	PE_LOG_ERROR("CPlayerInputPreserverを持たないゲームオブジェクトが指定されました。");
	return false;
}

void t_player_input_sender::update() {
	using input_code = c_player_input_observer::input_code;
	if (target_) {
		//キー入力伝達
		const auto &kim = *game::instance().input_manager();
		input_code::type input = 0;
		if (kim.key_state(keyboard::left) || kim.key_state(keyboard::a)) { input |= input_code::left; } else if (kim.
			key_state(keyboard::right) || kim.key_state(keyboard::d)) { input |= input_code::right; }
		if (kim.key_state(keyboard::up) || kim.key_state(keyboard::w)) { input |= input_code::up; } else if (kim.
			key_state(keyboard::down) || kim.key_state(keyboard::s)) { input |= input_code::down; }
		if (kim.key_state(keyboard::c)) { input |= input_code::jump; }
		if (kim.mouse_button_state(mouse_button::right)) { input |= input_code::physical_attack; }
		if (kim.mouse_button_state(mouse_button::left)) { input |= input_code::shot_attack; }
		target_->update_input(input);
		//カーソル位置伝達
		target_->update_pointer(kim.get_mouse_pointer_position());
	}
}
