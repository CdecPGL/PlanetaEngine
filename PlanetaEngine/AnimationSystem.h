#pragma once
#include "SceneModule.h"

namespace planeta_engine {
	namespace core {
		class AnimationSystem : public SceneModule {
		public:
			AnimationSystem();
			~AnimationSystem();
			bool Initialize()override;
			void Update()override;
			void Finalize()override;

			void UpdateAnimation();
		};
	}
}

