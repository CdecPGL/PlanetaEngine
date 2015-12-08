#pragma once
#include "Vector2D.h"

namespace planeta_engine {
	namespace utility {
		Vector2D<double> CovertPositionUISpaceToGameObjectSpace(const Vector2D<int>& ui_pos);
		Vector2D<int> CovertPositionGameObjectSpaceToUISpace(const Vector2D<double>& game_object_pos);
	}
}
