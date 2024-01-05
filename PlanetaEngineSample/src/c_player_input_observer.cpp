#include "c_player_input_observer.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(c_player_input_observer) { registerer; }

void c_player_input_observer::update_input(const input_code::type input) {
	input_down_ = input & ~input_state_;
	input_up_ = ~input & input_state_;
	input_state_ = input;
}

void c_player_input_observer::update_pointer(const vector_2di &scr_pos) { pointer_pos_ = scr_pos; }

void c_player_input_observer::reset_input() {
	input_state_ = 0;
	input_down_ = 0;
	input_up_ = 0;
}
