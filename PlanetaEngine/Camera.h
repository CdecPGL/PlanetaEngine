#pragma once

#include "Object.h"
#include "NewVector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Property.h"

namespace planeta_engine {
	class Camera : public core::Object {
	public:
		Camera();
		void Update();
		bool Initialize();
		enum class Mode{ Camera2D, Camera3D };
		void ChangeTo2DMode();
		void ChangeTo3DMode();
		Mode mode()const { return mode_; }
		Vector2Dd CovertPositionGUISpaceToGameObjectSpace(const Vector2Di& gui_space_pos)const;
		Vector2Di CovertPositionGameObjectSpaceToGUISpace(const Vector2Dd& gameobject_space_pos)const;
	private:
		Mode mode_ = Mode::Camera2D;
		Vector3Dd position_;
		const Vector3Dd& get_position()const { return position_; }
		void set_position(const Vector3Dd& pos);
		Vector4Dd rotation_;
		const Vector4Dd& get_rotation()const { return rotation_; }
		void set_rotatoin(const Vector4Dd& rota);
		Vector3Dd euler_angle_;
		const Vector3Dd& get_euler_angle_rad()const { return euler_angle_; }
		void set_euler_angle_rad(const Vector3Dd& ang);
		double get_angle_2d_rad()const { return euler_angle_.z; }
		void set_angle_2d_rad(double angle_rad);
		double scale_;
		double get_scale()const { return scale_; }
		void set_scale(double s);
	public:
		utility::ReadWritePropertyR<Camera, Vector3Dd, &Camera::get_position, &Camera::set_position> position;
		utility::ReadWritePropertyR<Camera, Vector4Dd, &Camera::get_rotation, &Camera::set_rotatoin> rotation;
		utility::ReadWritePropertyR<Camera, Vector3Dd, &Camera::get_euler_angle_rad, &Camera::set_euler_angle_rad> euler_angle_rad;
		utility::ReadWritePropertyV<Camera, double, &Camera::get_angle_2d_rad, &Camera::set_angle_2d_rad> angle_2d_rad;
		utility::ReadWritePropertyV<Camera, double, &Camera::get_scale, &Camera::set_scale> scale;
	};
}
