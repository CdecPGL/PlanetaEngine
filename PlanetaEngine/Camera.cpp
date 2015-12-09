#include "Camera.h"

#include "DxLib.h"

namespace planeta_engine {
	void Camera::ApplyToDxLib() {
		SetCameraPositionAndAngle(VGet((float)position_.x, (float)position_.y, position_.z), GetCameraAngleVRotate(), GetCameraAngleHRotate(), GetCameraAngleTRotate());

	}
}