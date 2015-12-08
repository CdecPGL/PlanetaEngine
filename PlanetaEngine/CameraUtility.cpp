#include "CameraUtility.h"
#include "DxLib.h"

namespace planeta_engine {
	namespace utility {
		
		math::Vector2Dd CovertPositionUISpaceToGameObjectSpace(const math::Vector2Di& ui_pos)
		{
			VECTOR gov = ConvScreenPosToWorldPos(VGet((float)ui_pos.x, (float)ui_pos.y, 0.0f));
			return math::Vector2Dd(gov.x, gov.y);
		}

		math::Vector2Di CovertPositionGameObjectSpaceToUISpace(const math::Vector2Dd& game_object_pos)
		{
			VECTOR uiv = ConvWorldPosToScreenPos(VGet((float)game_object_pos.x, (float)game_object_pos.y, 0.0f));
			return math::Vector2Di((int)uiv.x, (int)uiv.y);
		}

	}
}