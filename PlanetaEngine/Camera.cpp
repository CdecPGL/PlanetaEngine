#include <cassert>

#include "Camera.h"
#include "ConfigData.h"
#include "SystemLog.h"

#include "DxLib.h"

namespace planeta_engine {
	Camera::Camera(){}

	void Camera::Update() {
		
	}

	void Camera::ChangeTo2DMode() {
		mode_ = Mode::Camera2D;
		SetupCamera_Ortho((float)(core::config_data::engine::DrawSize().y / scale_));
		auto cp = GetCameraPosition();
		position_.Set(cp.x, cp.y, cp.z);
	}

	void Camera::ChangeTo3DMode() {
		assert(false); //������
	}

	Vector2Dd Camera::CovertPositionGUISpaceToGameObjectSpace(const Vector2Di& ui_pos)const {
		VECTOR gov = ConvScreenPosToWorldPos(VGet((float)ui_pos.x, (float)ui_pos.y, 0.0f));
		return Vector2Dd(gov.x, gov.y);
	}

	Vector2Di Camera::CovertPositionGameObjectSpaceToGUISpace(const Vector2Dd& game_object_pos)const {
		VECTOR uiv = ConvWorldPosToScreenPos(VGet((float)game_object_pos.x, (float)game_object_pos.y, 0.0f));
		return Vector2Di((int)uiv.x, (int)uiv.y);
	}

	bool Camera::Initialize() {
		ChangeTo2DMode();
		return true;
	}

	void Camera::position(const Vector3Dd& pos) {
		position_ = pos;
		switch (mode_) {
		case planeta_engine::Camera::Mode::Camera2D:
			SetCameraPositionAndAngle(VGet((float)position_.x, (float)position_.y, GetCameraPosition().z), GetCameraAngleVRotate(), GetCameraAngleHRotate(), GetCameraAngleTRotate());
			break;
		case planeta_engine::Camera::Mode::Camera3D:
			SetCameraPositionAndAngle(VGet((float)position_.x, (float)position_.y, (float)position_.z), GetCameraAngleVRotate(), GetCameraAngleHRotate(), GetCameraAngleTRotate());
			break;
		default:
			assert(false);
			break;
		}
	}

	void Camera::angle_2d_rad(double angle_rad) {
		euler_angle_.z = angle_rad;
		switch (mode_) {
		case planeta_engine::Camera::Mode::Camera2D:
			SetCameraPositionAndAngle(GetCameraPosition(), GetCameraAngleVRotate(), GetCameraAngleHRotate(), (float)euler_angle_.z);
			break;
		case planeta_engine::Camera::Mode::Camera3D:
			debug::SystemLog::instance().LogWarning("Camera3D�ɑ΂���angle_2d_rad�̕ύX�͖����ł��B", __FUNCTION__);
			break;
		default:
			assert(false);
			break;
		}
	}

	void Camera::euler_angle_rad(const Vector3Dd& ang) {
		euler_angle_ = ang;
		switch (mode_) {
		case planeta_engine::Camera::Mode::Camera2D:
			debug::SystemLog::instance().LogWarning("Camera2D�ɑ΂���euler_angle_rad�̕ύX�͖����ł��B", __FUNCTION__);
			break;
		case planeta_engine::Camera::Mode::Camera3D:
			SetCameraPositionAndAngle(GetCameraPosition(), (float)euler_angle_.x, (float)euler_angle_.y, (float)euler_angle_.z);
			break;
		default:
			assert(false);
			break;
		}
	}

	void Camera::scale(double s) {
		scale_ = s;
		switch (mode_) {
		case planeta_engine::Camera::Mode::Camera2D:
			SetupCamera_Ortho((float)(core::config_data::engine::DrawSize().y / scale_));
			break;
		case planeta_engine::Camera::Mode::Camera3D:
			assert(false); //������
			break;
		default:
			assert(false);
			break;
		}
	}

	void Camera::rotation(const Vector4Dd& rota) {
		rotation_ = rota;
		assert(false); //������
	}


}