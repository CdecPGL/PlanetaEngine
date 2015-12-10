#include <cassert>

#include "Camera.h"
#include "ConfigData.h"

#include "DxLib.h"

namespace planeta_engine {
	float x,y,z,vrota,hrota,trota;
	void Camera::ApplyToDxLib() {
		switch (mode_) {
		case Mode::Camera2D:
			SetupCamera_Ortho(core::config_data::engine::DrawSize().y / (float)scale);
			x = (float)position_.x;
			y = (float)position_.y;
			vrota = GetCameraAngleVRotate();
			hrota = GetCameraAngleHRotate();
			trota = (float)euler_angle_.z;
			break;
		case Mode::Camera3D:
			assert(false);
			break;
		default:
			assert(false);
			break;
		}
		SetCameraPositionAndAngle(VGet(x, y, z), vrota, hrota, trota);
	}

	void Camera::ChangeTo2DMode() {
		mode_ = Mode::Camera2D;
	}

	void Camera::ChangeTo3DMode() {
		assert(false);
	}

}