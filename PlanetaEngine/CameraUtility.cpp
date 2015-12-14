#include "CameraUtility.h"
#include "DxLib.h"

namespace planeta_engine {
	namespace utility {
		
		Vector2D<double> CovertPositionGUISpaceToGameObjectSpace(const Vector2D<int>& ui_pos)
		{
			VECTOR gov = ConvScreenPosToWorldPos(VGet((float)ui_pos.x, (float)ui_pos.y, 0.0f));
			return Vector2D<double>(gov.x, gov.y);
		}

		Vector2D<int> CovertPositionGameObjectSpaceToGUISpace(const Vector2D<double>& game_object_pos)
		{
			VECTOR uiv = ConvWorldPosToScreenPos(VGet((float)game_object_pos.x, (float)game_object_pos.y, 0.0f));
			return Vector2D<int>((int)uiv.x, (int)uiv.y);
		}

	}
}