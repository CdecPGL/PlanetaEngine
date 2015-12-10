#pragma once

#include "Object.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Property.h"

namespace planeta_engine {
	class Camera : public core::Object {
	public:
		void ApplyToDxLib();
		enum class Mode{ Camera2D, Camera3D };
		void ChangeTo2DMode();
		void ChangeTo3DMode();
		Mode mode()const { return mode_; }
	private:
		Mode mode_;
		Vector3Dd position_;
		const Vector3Dd& get_position()const { return position_; }
		void set_position(const Vector3Dd& pos) { position_ = pos; }
		Vector4Dd rotation_;
		const Vector4Dd& get_rotation()const { return rotation_; }
		void set_rotatoin(const Vector4Dd& rota) { rotation_ = rota; }
		Vector3Dd euler_angle_;
		const Vector3Dd& get_euler_angle()const { return euler_angle_; }
		void set_euler_angle(const Vector3Dd& ang) { euler_angle_ = ang; }
		double scale_;
		double get_scale()const { return scale_; }
		void set_scale(double s) { scale_ = scale; }
	public:
		utility::PropertyR<Camera, Vector3Dd, &Camera::get_position, &Camera::set_position> position;
		utility::PropertyR<Camera, Vector4Dd, &Camera::get_rotation, &Camera::set_rotatoin> rotation;
		utility::PropertyR<Camera, Vector3Dd, &Camera::get_euler_angle, &Camera::set_euler_angle> euler_angle;
		utility::PropertyV<Camera, double, &Camera::get_scale, &Camera::set_scale> scale;
	};
}
