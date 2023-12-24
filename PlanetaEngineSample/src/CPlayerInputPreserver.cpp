#include "CPlayerInputPreserver.hpp"

using namespace plnt;

PE_REFLECTION_DATA_REGISTERER_DEFINITION(CPlayerInputPreserver) {
	registerer;
}

void CPlayerInputPreserver::UpdateInput(const InputCode::Type input) {
	input_down_ = input & ~input_state_;
	input_up_ = ~input & input_state_;
	input_state_ = input;
}

void CPlayerInputPreserver::UpdatePointer(const Vector2Di& scr_pos) {
	pointer_pos_ = scr_pos;
}

void CPlayerInputPreserver::ResetInput() {
	input_state_ = 0;
	input_down_ = 0;
	input_up_ = 0;
}
