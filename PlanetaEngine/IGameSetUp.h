#pragma once

#include <string>
#include <list>

namespace planeta_engine {
	namespace core {
		class SceneManager;
		class CollisionGroupMatrix;
		class TaskKeyPositionDefinition;
		class IGameSetUp {
		public:
			virtual ~IGameSetUp() = 0 {};
			/*‰Šú‰»—pŠÖ”ŒQ*/
			virtual SceneManager& scene_manager() = 0;
			virtual CollisionGroupMatrix& RefCollisionGroupMatrix() = 0;
			virtual TaskKeyPositionDefinition& RefTaskKeyPositionDefinition() = 0;
		};
	}
}
