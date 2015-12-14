#include <cassert>

#include "Camera.h"
#include "ConfigData.h"

#include "DxLib.h"

namespace planeta_engine {
	Camera::Camera() :position(*this), rotation(*this), euler_angle(*this), scale(*this) {}

	void Camera::ApplyToDxLib() {
		float x(0), y(0), z(0), vrota(0), hrota(0), trota(0);
		switch (mode_) {
		case Mode::Camera2D:
			SetupCamera_Ortho((float)(core::config_data::engine::DrawSize().y / scale));
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

	Vector2Dd Camera::CovertPositionGUISpaceToGameObjectSpace(const Vector2Di& ui_pos)const {
		VECTOR gov = ConvScreenPosToWorldPos(VGet((float)ui_pos.x, (float)ui_pos.y, 0.0f));
		return Vector2Dd(gov.x, gov.y);
	}

	Vector2Di Camera::CovertPositionGameObjectSpaceToGUISpace(const Vector2Dd& game_object_pos)const {
		VECTOR uiv = ConvWorldPosToScreenPos(VGet((float)game_object_pos.x, (float)game_object_pos.y, 0.0f));
		return Vector2Di((int)uiv.x, (int)uiv.y);
	}
}