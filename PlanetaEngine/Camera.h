#pragma once

#include "Object.h"
#include "NewVector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"

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
		const Vector3Dd& position()const { return position_; }
		Camera& position(const Vector3Dd& pos);
		const Vector4Dd& rotation()const { return rotation_; }
		Camera& rotation(const Vector4Dd& rota);
		const Vector3Dd& euler_angle_rad()const { return euler_angle_; }
		Camera& euler_angle_rad(const Vector3Dd& ang);
		double angle_2d_rad()const { return euler_angle_.z; }
		Camera& angle_2d_rad(double angle_rad);
		double scale()const { return scale_; }
		Camera& scale(double s);
	private:
		Mode mode_ = Mode::Camera2D;
		Vector3Dd position_;
		Vector4Dd rotation_;
		Vector3Dd euler_angle_;
		double scale_;
	};
}
