#pragma once
#include "Vector2D.h"

namespace planeta_engine {
	namespace utility {
		math::Vector2Dd CovertPositionUISpaceToGameObjectSpace(const math::Vector2Di& ui_pos);
		math::Vector2Di CovertPositionGameObjectSpaceToUISpace(const math::Vector2Dd& game_object_pos);
	}
}
