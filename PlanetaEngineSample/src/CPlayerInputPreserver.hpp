#pragma once

#include "planeta/PlanetaEngine.hpp"

class CPlayerInputPreserver : public plnt::GameObjectStandardComponent {
	PE_REFLECTION_DATA_REGISTERER_DECLARATION(CPlayerInputPreserver);
public:
	using Super = plnt::GameObjectStandardComponent;
	struct InputCode {
		using Type = unsigned int;
		static constexpr Type None = 0;
		static constexpr Type Up = 1;
		static constexpr Type Down = 2;
		static constexpr Type Left = 4;
		static constexpr Type Right = 8;
		static constexpr Type Jump= 16;
		static constexpr Type PhysicalAttack = 32;
		static constexpr Type ShotAttack = 64;
		static constexpr Type Guard = 128;
	};
	void UpdateInput(const InputCode::Type input);
	void UpdatePointer(const plnt::Vector2Di& scr_pos);
	void ResetInput();

	InputCode::Type input_state()const { return input_state_; }
	InputCode::Type input_down()const { return input_down_; }
	InputCode::Type input_up()const { return input_up_; }
	plnt::Vector2Di pointer_position()const { return pointer_pos_; }
private:
	InputCode::Type input_up_;
	InputCode::Type input_down_;
	InputCode::Type input_state_;
	plnt::Vector2Di pointer_pos_;
};
PE_GAMEOBJECTCOMPONENT_CLASS(CPlayerInputPreserver);
