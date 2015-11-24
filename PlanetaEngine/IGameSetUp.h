#pragma once

#include <string>

namespace planeta_engine {
	namespace core {
		class SceneManager;
		class CollisionGroupMatrix;
		class IGameSetUp {
		public:
			virtual ~IGameSetUp() = 0 {};
			/*�������p�֐��Q*/
			virtual SceneManager& scene_manager() = 0;
			virtual CollisionGroupMatrix& RefCollisionGroupMatrix() = 0;
		};
	}
}
