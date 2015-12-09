#pragma once

#include "Object.h"
#include "Vector3D.h"
#include "Vector4D.h"

namespace planeta_engine {
	class Camera : public core::Object {
	public:
		void ApplyToDxLib();
	private:
		Vector3Dd position_;
		Vector4Dd rotation_;
		double scale_;
	};
}
