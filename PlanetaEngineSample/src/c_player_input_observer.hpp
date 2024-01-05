#pragma once

#include "planeta/planeta_engine.hpp"

class c_player_input_observer final : public plnt::game_object_standard_component {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(c_player_input_observer);

public:
	using super = game_object_standard_component;

	struct input_code {
		using type = unsigned int;
		static constexpr type none = 0;
		static constexpr type up = 1;
		static constexpr type down = 2;
		static constexpr type left = 4;
		static constexpr type right = 8;
		static constexpr type jump = 16;
		static constexpr type physical_attack = 32;
		static constexpr type shot_attack = 64;
		static constexpr type guard = 128;
	};

	void update_input(input_code::type input);
	void update_pointer(const plnt::vector_2di &scr_pos);
	void reset_input();

	input_code::type input_state() const { return input_state_; }
	input_code::type input_down() const { return input_down_; }
	input_code::type input_up() const { return input_up_; }
	plnt::vector_2di pointer_position() const { return pointer_pos_; }

private:
	input_code::type input_up_ = 0;
	input_code::type input_down_ = 0;
	input_code::type input_state_ = 0;
	plnt::vector_2di pointer_pos_;
};

PE_GAMEOBJECTCOMPONENT_CLASS(c_player_input_observer);
