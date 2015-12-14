#pragma once
#include "Vector2D.h"

namespace planeta_engine {
	namespace utility {
		Vector2D<double> CovertPositionGUISpaceToGameObjectSpace(const Vector2D<int>& ui_pos);
		Vector2D<int> CovertPositionGameObjectSpaceToGUISpace(const Vector2D<double>& game_object_pos);
	}
}
