#pragma once

#include "Object.h"

#include "Vector3D.h"

namespace planeta_engine {
	namespace core {
		class Camera : public Object{
		public:
			void ApplyToDxLib();
		private:
			math::Vector3D<double> position_;
		};
	}
}
