#pragma once

#include "i_transform_system.hpp"
#include "SceneModule.hpp"

namespace plnt {
	class CTransform2D;
	class CTransformGUI;

	namespace private_ {
		class TransformSystem : public i_transform_system, public scene_module {
		public:
			virtual ~TransformSystem() = 0 { };
			virtual void ApplyVelocity() = 0;
			virtual int RegisterTransform2D(CTransform2D *transform2d) = 0;
			virtual bool RemoveTransform2D(int id) = 0;
			virtual int RegisterTransformGUI(CTransformGUI *transformgui) = 0;
			virtual bool RemoveTransformGUI(int id) = 0;
		};
	}
}
