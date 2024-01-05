#pragma once

#include "planeta/planeta_engine.hpp"

class c_player_input_observer;

class t_player_input_sender final : public plnt::task {
public:
	using super = task;
	bool set_target(const plnt::weak_pointer<plnt::i_game_object> &game_object);

private:
	void update() override;
	plnt::weak_pointer<c_player_input_observer> target_;
};

PE_TASK_CLASS(t_player_input_sender);
