#pragma once

#include "ITransformSystem.h"
#include "SceneModule.h"

namespace planeta {
	class CTransform2D;
	namespace private_ {
		class TransformSystem : public ITransformSystem, public SceneModule{
		public:
			virtual ~TransformSystem() = 0 {};
			virtual void ApplyVelocity() = 0;
			virtual int RegisterTransform2D(CTransform2D* transform2d) = 0;
			virtual bool RemoveTransform2D(int id) = 0;
		};
	}
}