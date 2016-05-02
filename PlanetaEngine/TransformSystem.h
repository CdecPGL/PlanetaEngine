#pragma once

#include "SceneModule.h"

namespace planeta_engine {
	namespace core {
		class TransformSystem  : public SceneModule{
		public:
			TransformSystem();
			~TransformSystem();
			bool Initialize()override;
			void Update()override;
			void Finalize()override;

			void ExcuteCoordinateConvertion();
			void ApplyVelocity();
		};
	}
}
